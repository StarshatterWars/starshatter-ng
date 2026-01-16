#ifdef _WIN32
#include <shlwapi.h>
#else
#include <fnmatch.h>
#endif

#include <unistd.h>

#include <cstdint>
#include <exception>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <system_error>
#include <vector>

#include <Archive.h>


namespace fs = std::filesystem;


enum class Action
{
	NOTHING,
	LIST,
	BLOCKS,
	UPDATE,
	EXTRACT,
};


struct Options
{
	bool help {false};
	Action action {Action::NOTHING};
	bool create {false};
	const char* archive {nullptr};
	std::vector<std::string> files {};
};


class ArgsError : std::exception
{
public:
	ArgsError(int opt, const char* msg) : option {opt}, message {msg} {}
	int option;
	const char* message;
};


class ExtractError : std::exception {};


static Options ParseArgs(int argc, char* argv[]);
static void PrintUsage(std::ostream& out=std::cerr);
static void PrintHelp(std::ostream& out=std::cout);
static auto FilterFile(const std::vector<std::string>& patterns, std::function<void(const char*)> func) ->
	std::function<void(const char*)>;
static void UpdateFile(ArchiveEx::Archive& archive, const std::string& path, bool warn=true);
static void ExtractFile(ArchiveEx::Archive& archive, const char* filepath);


static const char* program {"dat"};


int
main(int argc, char* argv[])
try {
	program = argv[0];
	const auto opts = ParseArgs(argc, argv);
	if (opts.help) {
		PrintUsage(std::cout);
		PrintHelp();
		return 0;
	}
	ArchiveEx::Archive archive(opts.archive, opts.create);
	std::size_t i = 0;
	switch (opts.action) {
	case Action::NOTHING:
		break;
	case Action::LIST:
		archive.ForEachEntry(FilterFile(opts.files, [](const char* name){ std::cout << name << std::endl; }));
		break;
	case Action::BLOCKS:
		archive.ForEachBlock([&i](const char* name){ std::cout << i++ << "\t" << name << std::endl; });
		break;
	case Action::UPDATE:
		for (const auto& file : opts.files)
			UpdateFile(archive, file);
		break;
	case Action::EXTRACT:
		archive.ForEachEntry(FilterFile(opts.files, [&archive](const char* name){ ExtractFile(archive, name); }));
		break;
	}
}
catch (const ArgsError& err) {
	std::cerr << err.message;
	if (err.option > 0)
		std::cerr << ": " << static_cast<char>(err.option);
	std::cerr << std::endl;
	PrintUsage();
	return 1;
}
catch (const char* err) {
	std::cerr << err << std::endl;
	return 1;
}


Options
ParseArgs(int argc, char* argv[])
{
	int opt;
	Options opts;
	while (-1 != (opt = getopt(argc, argv, ":hlbuxc"))) {
		switch (opt) {
		case 'h':
			opts.help = true;
			return opts;  // No need to waste time as program won't do anything else anyway.
		case 'l':
			opts.action = Action::LIST;
			break;
		case 'b':
			opts.action = Action::BLOCKS;
			break;
		case 'u':
			opts.action = Action::UPDATE;
			break;
		case 'x':
			opts.action = Action::EXTRACT;
			break;
		case 'c':
			opts.create = true;
			break;
		case ':':
			throw ArgsError(optopt, "missing argument for option");
		default:
			throw ArgsError(optopt, "invalid option");
		}
	}
	if (optind >= argc)
		throw ArgsError(0, "missing archive name");
	opts.archive = argv[optind++];
	for (; optind < argc; ++optind)
		opts.files.emplace_back(argv[optind]);
	return opts;
}


void
PrintUsage(std::ostream& out)
{
	out << "Usage: " << program << " [-hlbuxc] archive [file [files...]]" << std::endl;
}


void
PrintHelp(std::ostream& out)
{
	out << "Options:" << std::endl;
	out << "  -h\tPrints this help message and exits." << std::endl;
	out << "  -l\tLists files in the archive." << std::endl;
	out << "  -b\tLists content of each block in archive." << std::endl;
	out << "  -u\tInserts files from file system into archive." << std::endl;
	out << "  -x\tExtracts files from the archive." << std::endl;
	out << "  -c\tCreate the archive if it does not exist." << std::endl;
}


std::function<void(const char*)>
FilterFile(const std::vector<std::string>& patterns, std::function<void(const char*)> func)
{
	return [&patterns, func](const char* name) -> void {
		if (patterns.empty())
			return func(name);
		for (const auto& pattern : patterns) {
#ifdef _WIN32
			if (PathMatchSpecA(name, pattern.c_str()))
#else
			if (0 == fnmatch(pattern.c_str(), name, FNM_PATHNAME | FNM_LEADING_DIR))
#endif
				return func(name);
		}
	};
}


void
UpdateFile(ArchiveEx::Archive& archive, const std::string& path, bool warn)
{
	const auto status = fs::status(path);
	switch (status.type()) {
	case fs::file_type::regular:
		archive.Insert(path.c_str());
		break;
	case fs::file_type::directory:
		for (const auto& entry : fs::directory_iterator{path})
			UpdateFile(archive, entry.path().string(), false);
		break;
	default:
		if (warn)
			std::cerr << "unsupported file type: " << path << std::endl;
		break;
	}
}


void
ExtractFile(ArchiveEx::Archive& archive, const char* filepath)
try {
	fs::path path{filepath};
	std::uint8_t* tmp{nullptr};
	const int length = archive.Extract(filepath, tmp);
	if (0 > length)
		throw ExtractError();
	std::unique_ptr<std::uint8_t[]> buf{tmp};
	std::error_code err{};
	if (path.has_parent_path())
		fs::create_directories(path.parent_path(), err);
	if (err)
		throw ExtractError();
	std::ofstream file{path};
	if (!file.is_open())
		throw ExtractError();
	file.write(reinterpret_cast<const char*>(buf.get()), length);
}
catch (const ExtractError& err) {
	std::cerr << "could not extract: " << filepath << std::endl;
}
