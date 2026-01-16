/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Implementation of the Dictionary class
*/

#ifndef NDEBUG
#define DICT_CHECK(a, b) if ((a) == 0) throw b;
#else
#define DICT_CHECK(a, b)
#endif

const int CHAINS = 256;

// +-------------------------------------------------------------------+

template <class T> Dictionary<T>::Dictionary()
   : items(0)
{ init(); }

template <class T> Dictionary<T>::~Dictionary()
{ clear(); }

// +-------------------------------------------------------------------+

template <class T>
void Dictionary<T>::init()
{
   items = 0;
   memset(table, 0, CHAINS*sizeof(PTR));
}

template <class T>
void Dictionary<T>::clear()
{
   for (int i = 0; i < CHAINS; i++) {
      DictionaryCell<T>* link = table[i];

      while (link) {
         DictionaryCell<T>* n = link->next;
         delete link;
         link = n;
      }
   }

   init();
}

// +-------------------------------------------------------------------+

template <class T>
T& Dictionary<T>::operator[](const Text& key)
{
   int idx = key.hash() % CHAINS;
   DictionaryCell<T>* cell = table[idx];

   if (cell == 0) {  // empty chain
      items++;
      cell = new DictionaryCell<T>(key);
      table[idx] = cell;
      return cell->value;
   }
   else {            // search for key
      while (cell->next && cell->key != key)
         cell = cell->next;

      if (cell->key != key) {    // not found in chain
         items++;
         cell->next = new DictionaryCell<T>(key);
         return cell->next->value;
      }
      else {         // found: return it!
         return cell->value;
      }
   }
}

// +-------------------------------------------------------------------+

template <class T>
void Dictionary<T>::insert(const Text& key, const T& val)
{
   T& value = operator[](key);
      value = val;
}

// +-------------------------------------------------------------------+

template <class T>
void Dictionary<T>::remove(const Text& key)
{
   int idx = key.hash() % CHAINS;
   DictionaryCell<T>* cell = table[idx];

   if (cell == 0) {  // empty chain
      return;
   }
   else {            // search for key
      while (cell->next && cell->key != key)
         cell = cell->next;

      if (cell->key != key) {    // not found in chain
         return;
      }
      else {         // found: remove it!
         if (table[idx] == cell) {
            table[idx] = cell->next;
            delete cell;
         }
         else {
            DictionaryCell<T>* p = table[idx];
            while (p->next != cell)
               p = p->next;
            p->next = cell->next;
            delete cell;
         }
      }
   }
}

// +-------------------------------------------------------------------+

template <class T>
int Dictionary<T>::contains(const Text& key) const
{
   int idx = key.hash() % CHAINS;
   DictionaryCell<T>* cell = table[idx];

   if (cell != 0) {
      while (cell->next && cell->key != key)
         cell = cell->next;

      if (cell->key == key)
         return 1;
   }

   return 0;
}

// +-------------------------------------------------------------------+

template <class T>
T Dictionary<T>::find(const Text& key, T defval) const
{
   int idx = key.hash() % CHAINS;
   DictionaryCell<T>* cell = table[idx];

   if (cell != 0) {
      while (cell->next && cell->key != key)
         cell = cell->next;

      if (cell->key == key)
         return cell->value;
   }

   return defval;
}

// +-------------------------------------------------------------------+

template <class T> DictionaryIter<T>::DictionaryIter(Dictionary<T>& d)
   : dict(&d), chain(0), here(0)
{ }

template <class T> DictionaryIter<T>::~DictionaryIter()
{ }

// +-------------------------------------------------------------------+

template <class T>
void DictionaryIter<T>::reset()
{
   chain = 0;
   here  = 0;
}

// +-------------------------------------------------------------------+

template <class T>
Text DictionaryIter<T>::key() const
{
   return here->key;
}

template <class T>
T DictionaryIter<T>::value() const
{
   return here->value;
}

// +-------------------------------------------------------------------+

template <class T>
int DictionaryIter<T>::operator++()
{
   forth();
   int more = chain < CHAINS;
   return more;
}

// +-------------------------------------------------------------------+

template <class T>
void DictionaryIter<T>::forth()
{
   if (here) {
      here = here->next;
      if (!here) // off the end of this chain
         chain++;
   }

   if (!here) {
      while (!dict->table[chain] && chain < CHAINS)
         chain++;

      if (chain < CHAINS)
         here = dict->table[chain];
      else
         here = 0;
   }
}

// +-------------------------------------------------------------------+

template <class T>
void DictionaryIter<T>::attach(Dictionary<T>& d)
{
   dict = &d;
   reset();
}

// +-------------------------------------------------------------------+

template <class T>
Dictionary<T>& DictionaryIter<T>::container()
{
   return *dict;
}

