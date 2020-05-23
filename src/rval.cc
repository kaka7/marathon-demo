#include <iostream>
using namespace std;
#include <iostream>
#include <cstring> // 为了使用内存复制函数memcpy()
using namespace std;

class MemoryBlock
{
public:
    // 默认构造
    MemoryBlock(const unsigned int nSize)
    {
        cout << "默认构造函数被调用,创建对象，申请内存资源" << nSize << "字节" << endl;
        m_nSize = nSize;
        m_pData = new char[nSize];
    }
    ~MemoryBlock()
    {
        if (0 != m_nSize) // 如果拥有内存资源
        {
            cout << "析构函数被调用,销毁对象，释放内存资源" << m_nSize << "字节";
            delete[] m_pData;
            m_nSize = 0;
        }
        cout << endl;
    }
    // 拷贝构造
    // MemoryBlock(MemoryBlock &other)
    //赋值构造 赋值操作符，完成对象的复制 这里的参数是一个左值引用
    MemoryBlock &operator=(const MemoryBlock &other)
    {
        // 判断是否自己给自己赋值
        if (this == &other)
            return *this;
        // 第一步，释放已有内存资源
        cout << "赋值构造函数被调用,释放已有内存资源" << m_nSize << "字节" << endl;
        delete[] m_pData;
        // 第二步，根据赋值对象的大小重新申请内存资源
        m_nSize = other.GetSize();
        cout << "重新申请内存资源" << m_nSize << "字节" << endl;
        m_pData = new char[m_nSize];
        // 第三步，复制数据
        cout << "复制数据" << m_nSize << "字节" << endl;
        memcpy(m_pData, other.GetData(), m_nSize);
        return *this;
        // 第四部,赋值构造调用完毕后再自己调用析构函数释放other的空间
    }
    //移动构造 可以接收右值引用为参数的移动构造函数
    MemoryBlock(MemoryBlock &&other) //不能有const
    {
        cout << "移动构造函数被调用,移动资源" << other.m_nSize << "字节" << endl;
        // 将目标对象的内存资源指针直接指向源对象的内存资源
        // 表示将源对象内存资源的管理权移交给目标对象
        m_pData = other.m_pData;
        m_nSize = other.m_nSize; // 复制相应的内存块大小
                                 // 将源对象的内存资源指针设置为nullptr
        // 表示这块内存资源已经归目标对象所有
        // 源对象不再拥有其管理权
        other.m_pData = nullptr;
        other.m_nSize = 0; // 内存块大小设置为0
    }
    // 移动赋值 可以接收右值引用为参数的赋值操作符
    MemoryBlock &operator=(MemoryBlock &&other)
    {
        // 第一步，释放已有内存资源
        delete[] m_pData;

        cout << "移动赋值被调用 释放已有资源" << m_nSize << "字节" << endl;
        // 第二步，移动资源，也就是移交内存资源的管理权
        cout << "移动资源,移交内存资源的管理权" << other.m_nSize << "字节" << endl;
        m_pData = other.m_pData;
        m_nSize = other.m_nSize;
        other.m_pData = nullptr;
        other.m_nSize = 0;
        return *this;
    }

public:
    unsigned int GetSize() const
    {
        return m_nSize;
    }
    char *GetData() const
    {
        return m_pData;
    }

private:
    unsigned int m_nSize;
    char *m_pData;
};

MemoryBlock CreateBlock(const unsigned int nSize)
{
    MemoryBlock mem(nSize);
    char *p = mem.GetData();
    memset(mem.GetData(), 'A', mem.GetSize()); //p[0] = 'A';
    return mem;
}

int main()
{
    MemoryBlock block(256);    //默认构造
    block = CreateBlock(1024); //默认构造+赋值构造(有移动构造时就用移动构造)

    // MemoryBlock block1(block); //拷贝构造被调用,此处没实现
    // MemoryBlock block2=  block;// todo
    //    cout << "移动构造的后,创建的对象大小是"
    //          << block.GetSize() << "字节" << endl;
    //     MemoryBlock block1(std::move(block)); //移动赋值构造被调用
    //     cout << "移动构造完毕后,原来的对象大小"
    //          << block.GetSize() << "字节" <<",新对象大小"<<block1.GetSize() << endl;

    return 0;
}

// 右值引用的两大用途：

//     移动语意 a=func();返回的结果通过"="操作符,首先清空a的内存,然后申请a的空间,赋值给a,最后销毁func()的中间结果
//     完美转发

/*
移动语意

移动语意(std::move)，可以将左值转化为右值引用

int a = 1; // 左值
int &b = a; // 左值引用

// 移动语意: 转换左值为右值引用
int &&c = std::move(a); 

void printInt(int& i) {
  cout << "lval ref: " << i << endl; 
}
void printInt(int&& i) {
  cout << "rval ref: " << i << endl; 
} 

int main() {
  int i = 1;
  
  // 调用 printInt(int&), i是左值
  printInt(i);
  
  // 调用 printInt(int&&), 6是右值
  printInt(6);
  
  // 调用 printInt(int&&)，移动语意
  printInt(std::move(i));   
}


*/

// 为啥要用移动语义?
// class myVector
// {
//     int size;
//     double *array;

// public:
//     // 拷贝构造函数
//     myVector(const myVector &rhs)
//     {
//         std::cout << "Copy Construct\n";
//         size = rhs.size;
//         array = new double[size];
//         for (int i = 0; i < size; i++)
//         {
//             array[i] = rhs.array[i];
//         }
//     }
//     ~myVector() { cout << "调用析构\n"; }

//     myVector(int n)
//     {
//         cout << "调用默认构造函数\n";
//         size = n;
//         array = new double[n];
//     }

//     // 移动构造函数
//     myVector(myVector &&rhs)
//     {
//         std::cout << "Move Constructor\n";
//         size = rhs.size;
//         array = rhs.array;
//         rhs.size = 0;
//         rhs.array = nullptr;
//     }
//     // 有这个函数时
//     //     调用默认构造函数
//     // Move Constructor
//     // 调用析构
//     // Move Constructor
//     // 调用析构
//     // 调用析构
// };
// void foo(myVector &&v)
// {
//     /* Do something */
//     //   cout<<"size:"<<v.size
//     cout << "call foo\n";
// }
// void foo(myVector &v)
// {
//     /* Do something */
//     //   cout<<"size:"<<v.size
//     cout << "call foo\n";
// }

// // 假设有一个函数，返回值是一个 MyVector
// myVector createMyVector()
// {
//     // myVector vec(2);
//     return myVector(2);
// };

// int main()
// {
//     // Case 1:
//     myVector reusable = createMyVector(); //改成void foo(myVector& v)
//     //     调用默认构造函数
//     // Copy Construct
//     // 调用析构
//     // Copy Construct
//     // 调用析构
//     // 调用析构

//     //     // 调用两次的原因:一次是在函数中myVector(2),另一次是在main中赋值

//     //     // // 这里会调用 myVector 的复制构造函数
//     //     // // 如果我们不希望 foo 随意修改 reusable
//     //     // // 这样做是 ok 的
//     //     // foo(reusable);
//     //     /* Do something with reusable */

//     //     // Case 2:
//     //     // createMyVector 会返回一个临时的右值
//     //     // 传参过程中会调用拷贝构造函数
//     //     // 多余地被复制一次
//     //     // 虽然大部分情况下会被编译器优化掉
//     //     foo(createMyVector()); //使用 void foo(myVector&& v) 少一次调用构造函数
// }
    // // g++ -std=c++11 src/test_rval.cc -fno-elide-constructors  -lpthread && time ./a.out


