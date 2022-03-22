# InIpp
Ini++ (Or InIpp) is just normal InI, but with changes and additions. <br>
This implemention is written in C++.

# Syntax

## Sections
Every value has to be in a section. <br>
A section is defined as this:
```
[SectionName]
```
Spaces are supported, but not recomended! <br>
Now the key-value pairs can be added:
```
[Section]
key="value"
int=1245
float=6.27
...
```
When pairs are not in a section, like this:
```
a="b"
[Section]
...
```
they will be merged to the "Main" section at the top, once the file has been written. <br>
It is possible to open a section more then once!
## Types
Every value of a key has a type. <br>
Known types are:
 - Null
 - String
 - Int
 - Float
 - Vector
 - List
 - Dictionary <br>
As defined in `IniTypes`.
<br>
These types will be checked for the user. <br>
<br>
<br>
Warning! something like:

```ini
[Section]
a=b
```

shoudln't work, because `b` is not a known type. It should be `"b"`! <br>
Luckily, Ini++ already handles this case for you. <br>
`b` would be turned to `"b"`. But be careful! `1` will always be an `int`, even when it was intended to be a string!
### Element
As defined in the `IniElement` class, it's a storage class for every known type.

### List
A list begins and ends with `[]`. <br>
A possible list is:
```
[10,"hello",(12,56,89),[sublist,12]]
```
A list can hold any type, also sublists work. <br>
`IniList` is defined as `std::vector<IniElement>`.

### Dictionary
A dictionary begins and ends with `{}` <br>
A possible dictionary is:
```
{key:value,41:"string",K:[list]}
```
A key has always to be a string or a number. The value can be any `IniType` <br>
`IniDictionary` is defined as `std::map<std::string,IniElement>`

### Vector
A vector begins and ends with `()` <br>
It has three values: `x`,`y` and `z`. Everyone is an int. <br>
A possible vector would be:
```
(14,685432,1279)
```
Floats are not yet supported. <br>
`IniVector` is defined as `struct IniVector { ... };`

## How to use
An example: (C++)
```c++
#include <iostream>
#include <fstream>
#include "Inipp.hpp"

int main() {

    IniFile file = IniFile::from_file("test.inipp");
    
    if(!file) {
        std::cout << file.error_msg() << "\n"; // An error occured!
        return 1;
    }
    
    IniVector vector = file.get("vector","section_a").to_vector(); // file.get() returns an IniElement
    
    std::cout << vector.x << " | " << vector.y << " | " << vector.z << "\n"; // Prints the vector

    vector.x += 10; // modifies the vector

    file.set("vector",vector,"section_a"); // apply changes

    vector = file.get("vector","section_a"); // gets the new vector

    IniList list = file.get("list","section_b"); // if not a list, returns empty list and sets error()
    IniDictionary dictionary = file.get("dictionary","section_b"); // if not a dictionary, returns empty dictionary and sets error()

    dictionary["key"] = list.back(); // sets key to the last element in the list
    list.pop_back(); // deletes the last element

    
    // writes everything back
    file.set("list",list,"section_b");
    file.set("dictionary",dictionary,"section_b");

    // write back to file
    file.to_file("test.inipp");
}
```

### Inipp itself
```ini
[Section]
key="value"
other_key="string value"
float=67.2456

[Section2]
list=["element1","element2",2744]
dict={key:"value",wow:["such","cool"]}
vector3=(1,23,456)
```
