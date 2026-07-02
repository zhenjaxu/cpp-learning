```cpp
class FileSystem{
public:
    static FileSystem& instance(){
        // 惰性初始化
        if(instance_==nullptr) instance_=new FileSystem();
        return *instance_;
    }

private:
    FileSystem(){}

    static FileSystem* instance_;
};
```

```cpp
class FileSystem{
public:
    static FileSystem& instance(){
        static FileSytem* instance=new FileSystem();
        return *instance;
    }

private:
    FileSystem(){}
};
```

```cpp
class FileSystem{
public:
    virtual ~FileSystem(){}
    virtual char* readFile(char* path)=0;
    virtual void writeFile(char* paht, char* contents)=0;
};
```
```cpp
class PS3FileSystem:public FileSystem{
public:
    virtual char* readFile(char* path){
        // 使用索尼的文件读写API...
    }

    virtual void writeFile(char* path, char* contents){
        // 使用索尼的文件读写API...
    }
};

class WiiFileSystem:public FileSystem{
public:
    virtual char* readFile(char* path){
        // 使用任天堂的文件读写API...
    }

    virtual void writeFile(char* path, char* contents){
        // 使用任天堂的文件读写API...
    }
};
```

```cpp
class FileSystem{
public:
    static FileSystem& instance();

    virtual ~FileSystem(){}
    virtual char* readFile(char* path)=0;
    virtual void writeFile(char* path, char* contents)=0;

protected:
    FileSystem(){}
};
```
```cpp
FileSystem& FileSystem::instance(){
    #if PLATFORM==PLAYSTATION3
        static FileSystem* instance=new PS3FileSystem();
    #elif PLATFORM==WII
        static FileSystem* instance=new WiiFileSystem();
    #endif

    return *instance;
}
```