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

修正了优先级队列的一些bug

day12

修正了deque的bug，上传了stl_queue.h和stl_stack.h两个文件，队列和栈都是基于deque实现的。

day13

更新了vector的一些内容，上传了stl_heap.h和stl_priority_queue.h两个文件。

day14

上传了红黑树。

day15

修复了红黑树插入时未初始化节点导致的段错误。

day16

上传了set、multiset、map、mutiset，四个容器均对红黑树进行了封装

day17

修复了set、multiset、map、mutiset bug

day18

上传了stl_numeric.h和部分stl_algobase.h

day19

更新了stl_algobase.h的一部分算法

day20

更新了stl_algobase.h的一部分算法，search待实现，reverse_iterator待实现

day21

更新了stl_algobase.h的一部分算法

day22

更新了stl_algobase.h的一部分算法

day23

更新了stl_algobase.h的一部分算法

day23

完结了stl_algobase.h，上传了stl_function.h文件，同时更新了multimap，map，RBtree


day24

上传了线程安全的栈以及测试代码



day25

上传了线程安全的队列以及测试代码

day26 rbtree
 增加一个节点，节点颜色默认为红色，对红黑树进行二分查找，对比，
 直至整个红黑树的叶子节点，确定当前节点插入红黑树的初始位置（插入节点
 初始位置为整个红黑树的叶子节点）

	1、判断插入节点是否为根节点，如果是根节点
		把当前节点置为根节点，并把颜色置为黑色
	2、判断父节点的颜色，如果父节点的颜色为黑色
		不用处理，不违反红黑树的性质
	3、父节点是红色，叔叔节点也是红色
		3.1、将父节点和叔叔节点设置为黑色，祖父节点设置为红色
		3.2、以祖父节点为插入节点，在进行判断
	4、父节点是红色，叔叔节点是黑色
		4.1、左左插入
			父节点变成黑色，祖父节点变成红色，祖父节点右旋
		4.2、左右插入
 	    	父节左旋，之后父节点就变成了子节点
	 	 	按照父节点为当前插入的红色节点，进行左左插入处理
		4.3、右右插入
			父节点变成黑色，祖父节点变成红色，祖父节点左旋
		4.4、右左插入
 	    	父节点右旋，之后父节点变成了子节点
 	    	按照父节点为当前插入的红色节点，进行右右插入处理


 总结：
	删除节点时，先看删除节点的颜色，再看兄弟节点的颜色，再看侄子节点的颜色（先看远侄子，再看近侄子）
最后看父节点的颜色
	当前节点->兄弟节点->远侄子节点->近侄子节点->父节点
	（补充）删除节点分两种情况，一种是叶子节点，一种不是叶子节点，如果不是叶子节点，进行和子节点值
的交换，直至节点没有叶子节点，变成删除叶子节点
	1、删除的是叶子节点，并且叶子节点为红色
		直接删除，不需要后续处理
	2、删除的是叶子节点，并且叶子节点为黑色
		需要处理
	3、删除节点下有一个子节点
		将当前删除的节点和子节点的值进行交换，就变成了删除叶子节点
		3.1、如果叶子节点是红色，对应情况1，直接删除
		3.2、如果叶子节点是黑色，对应情况2，进行后续处理
	4、删除节点有两个子节点
		将当前节点和后续节点中的一个节点值进行交换，改为删除叶子节点
		4.1、没有叶子节点
			对应情况1、2
		4.2、有一个叶子节点
			对应情况3
		4.2、有两个叶子节点
			对应情况4
	经过上述步骤的转换，将情况转换为删除叶子节点，其中叶子节点为红色的已经处理过，现在只需要考虑，
删除叶子节点为黑色的情况
	5、删除的叶子节点为黑色
		5.1、删除节点的兄弟节点为红色
			删除节点为左节点：
				将父节点和兄弟节点颜色互换，对父节点进行左旋
			删除节点为右节点：
				将父节点和兄弟节点颜色互换，对父节点进行右旋
		5.2、删除节点兄弟节点为黑色，远侄子节点为红色
			删除节点为左节点：
				这时删除节点的远侄子节点为兄弟节点的右节点
				将父节点和兄弟节点颜色对调，并把远侄子节点变成黑色，对父节点进行左旋
				删除当前需要删除的节点
			删除节点为右节点：
				这时删除节点的远侄子节点为兄弟节点的左节点
				将父节点和兄弟节点颜色对调，并把远侄子节点变成黑色，对父节点进行右旋
				删除当前需要删除的节点
		5.3、删除节点兄弟节点是黑色，远侄子节点是红色
			删除节点为左节点：
				近侄子节点和兄弟节点颜色互换，并将近侄子节点进行右旋
				这时候就变成了5.2情况
			删除节点为右节点：
				近侄子节点和兄弟节点颜色互换，将近侄子节点进行左旋
				这时候就变成了5.2情况
		5.4、父节点是红色，兄弟节点和兄弟节点的两个孩子（只能是空节点）都是黑色的情况
			将父节点变成黑色，兄弟节点变成红色
			删除当前节点
		5.5、父节点和兄弟节点及兄弟节点的两个子节点，都是黑色
			将兄弟节点变成红色，删除节点
			这样删除节点后，父节点的左右两个黑色节点数就相等了，但是经过祖父节点的路径黑色节点
			的数就少1个，这个时候，我们再以父节点为起始节点（不用删除节点了），继续根据情况进行
			平衡操作，在看是哪种情况，在进行对应的调整，这样一直向上，直到新的起始节点为根节点

day 27
加入了线程池。该线程池为异步实现，具有公共任务列表，每个线程有自己独有的列表，可以在某个具体的线程投放任务，线程可以偷取其他线程任务。


day28添加了中断线程
