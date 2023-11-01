# tinystl
Write your own miniature STL library from scratch

day 1

为了方便起见，将源文件和头文件放在了两个文件夹。
写了一个简单的控件配置器，JJ::allocator，该分配器是入门分配器，缺点很多，不建议使用，且在很多编译器中无法使用。

day2

编写构造和析构工具：construct() 和destroy()
__type_traits<T>和value_type()后续会实现。
第二个版本的destroy()函数使用萃取技术。当迭代器所指向之物可以析构的时候进行析构，反正什么也不做。
