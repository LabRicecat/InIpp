#ifndef INI_HANDLER_HPP
#define INI_HANDLER_HPP

#include <string>
#include <vector>
#include <map>

#define IS_INI_TYPE //marks which is a type and which is not

enum class IniType {
    Null,
    String,
    List,
    Int,
    Float,
    Vector,
    Dictionary,
};

inline std::string IniType2str(IniType type) {
    std::string types[] = {
        "Null", "String", "List", "Int", "Float","Vector","Dictionary"
    };
    return types[static_cast<int>(type)];
}

// Not a type but container for any type
class IniElement;
using IniDictionary     IS_INI_TYPE  =  std::map<std::string,IniElement>;
using IniList           IS_INI_TYPE  =  std::vector<IniElement>;
struct IniVector        IS_INI_TYPE 
{ // Vector3, not std::vector
    int x = 0,y = 0,z = 0;

    void operator=(IniVector v) {
        x = v.x;
        y = v.y;
        z = v.z;
    }

    std::string to_string() const { return "(" + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + ")"; }
};

namespace IniHelper {
    void set_type(IniElement& obj, IniType type);

    // @exception will return IniDictionary()
    IniDictionary to_dictionary(std::string source);
    // @exception will return IniList()
    IniList to_list(std::string source);
    // @exception will return IniVector()
    IniVector to_vector(std::string source);
    // @exception will return IniElement() (aka Null)
    IniElement to_element(std::string src);

    

    namespace tls {
        std::vector<std::string> split_by(std::string str,std::vector<char> erase, std::vector<char> keep = {}, std::vector<char> extract = {}, bool ignore_in_braces = false, bool delete_empty = true);

        template<typename T>
        inline bool isIn(T element, std::vector<T> vec) {
            for(auto i : vec) {
                if(element == i) {
                    return true;
                }
            }
            return false;
        }
    }

    std::string to_string(IniList list);
    std::string to_string(IniDictionary dictionary);
    std::string to_string(IniVector vector);
}

class IniElement {
    IniType type;
    std::string src;
public:
    IniType getType() const;

    IniElement(IniType type, std::string src) 
        : type(type), src(src) {}
    
    IniElement(IniType type) 
        : type(type), src("") {}
    
    //IniElement(IniElement& element) 
    //    : type(element.type), src(element.src) {}
    
    IniElement() {}

    std::string to_string() const;
    IniVector to_vector() const;
    IniList to_list() const;
    IniDictionary to_dictionary() const;

    static IniElement from_vector(IniVector vec);
    static IniElement from_list(IniList list);
    static IniElement from_dictionary(IniDictionary dictionary);

    IniElement operator=(IniList list);
    IniElement operator=(IniVector vector);
    IniElement operator=(IniDictionary dictionary);

    operator IniList();
    operator IniVector();
    operator IniDictionary();
    operator std::string();
};

std::ostream &operator<<(std::ostream& os, IniElement element);   

enum class IniError {
    OK,
    READ_ERROR,
    WRITE_ERROR,
    SYNTAX_ERROR
};

struct IniPair {
    std::string key;
    IniElement element = IniElement(IniType::Null);

    IniPair() {}
    IniPair(std::string key) : key(key) {}
    IniPair(std::string key, IniElement element)
        : key(key), element(element) {}
    //IniPair(IniPair& pair)
    //    : key(pair.key), element(pair.element) {}
};

struct IniSection {
    std::string name;
    std::vector<IniPair> members;

    IniSection(std::string name)
        : name(name) {}

    IniElement& operator[](std::string key);

    bool has(std::string key) const;

    std::string to_string() const;
};

class IniFile {
    IniError err = IniError::OK;
    std::string err_desc = "";
public:
    std::vector<IniSection> sections;

    bool has(std::string key) const;
    bool has(std::string key, std::string section) const;
    bool has_section(std::string section) const;
    IniError to_file(std::string file);

    static IniFile from_file(std::string file);

    void operator=(IniFile file);
    operator bool();
    IniError error() const;
    std::string error_msg() const;

    // @exception will return sections.front().members.front().element and sets err/err_desc
    IniElement& get(std::string key, std::string section = "Main");
    // @exception will return sections.front() and sets err/err_desc
    IniSection& section(std::string name);

    void set(std::string key, IniElement value, std::string section = "Main");
    void set(std::string key, IniList value, std::string section = "Main");
    void set(std::string key, IniDictionary value, std::string section = "Main");
    void set(std::string key, IniVector value, std::string section = "Main");

    IniFile(std::string file) {
        operator=(from_file(file));
    }

    IniFile() {}
};


#endif