# tinystl
Write your own miniature STL library from scratch

day 1

为了方便起见，将源文件和头文件放在了两个文件夹。
写了一个简单的控件配置器，JJ::allocator，该分配器是入门分配器，缺点很多，不建议使用，且在很多编译器中无法使用。

day2

编写构造和析构工具：construct() 和destroy()
__type_traits<T>和value_type()后续会实现。
第二个版本的destroy()函数使用萃取技术。当迭代器所指向之物可以析构的时候进行析构，反正什么也不做。

std_alloc.h里面定义了分配器，目前只定义了一级分配器。是否定了__USE_MALLOC宏决定alloc是一级还是二级分配器。

day3

增加了二级分配器，该分配器维持一个freelist表，用来分配8-128字节大小的内存，同时维持一个内存池。用来补充freelist表
如果内存池耗尽了，会申请分配，如果分配失败，会压榨freelist的内存，如果还是没有内存的话，只能寄希望于一级分配器了。
