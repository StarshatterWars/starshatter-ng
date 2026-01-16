# See AddVersionFile.cmake
include(GitDescribe)
include(GitToResource)
git_describe(STARSHATTER_VERSION)
git_to_resource(STARSHATTER_VERSION_RC ${STARSHATTER_VERSION})
configure_file("${TEMPLATE}" "${OUTPUT}" @ONLY)
