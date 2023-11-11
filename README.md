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

day4

修复了stl_alloc.h的一些错误，现在分配器可以直接使用（但不适配现代版本的stl）
上传了新文件stl_uninitialized.h，里面包含三个模板函数uninitialized_copy，uninitialized_fill，uninitialized_fill_n
用于在一块开辟的内存上构造对象，所有的的函数都使用了萃取方法，根据要构造对象有无拷贝构造函数来调用相应的函数。

day5

上传了stl_iterator.h文件，编写了五种迭代器。并利用写出了萃取机iterator_traits，利用萃取技术完成distance函数和advance
函数。更正了stl_uninitialized.h和std_construct.h。
添加了type_traits.h。用来提供__true_type和__false_type。同时，type_traits用来判断一个对象有没有默认构造，拷贝构造、析构函数等

day6

修正了stl_uninitialized.h的一些错误，以及std_construct.h中的一些错误。上传了stl_vector.h，该文件定义了vector容器。经过测试
该容器运行正常。

day7

提交了stl_list.h

day8

修正了list的一些bug，但任然有bug。等时间充足是修复。

day9

list已经可以正常使用
上传了stl_deque.h

day10

修复了deque的bug，修正了stl_uninitialized.h的一些bug。添加了deque的测试代码。

day11

修正了deque的bug，上传了stl_queue.h和stl_stack.h两个文件，队列和栈都是基于deque实现的。

day12
更新了vector的一些内容，上传了stl_heap.h和stl_priority_queue.h两个文件。
