#pragma once
//һԪ����
template<class Arg, class Result>
struct unary_fuction
{
	typedef Arg argument_type; //����
	typedef Result result_type; //��������
};

//��Ԫ����
template<class Arg1, class Arg2, class Result>
struct binary_function
{
	typedef Arg1 first_argument_type;
	typedef Arg2 second_argument_type;
	typedef Result resutlt_type;
};

//�ӡ������ˡ�����ȡģ����
template<class T>
struct plus : public binary_function<T, T, T>
{
	T operator()(const T& x, const T& y)const { return x + y; }
};

template<class T>
struct minus : public binary_function<T, T, T>
{
	T operator()(const T& x, const T& y)const { return x - y; }
};

template<class T>
struct multiplies : public binary_function<T, T, T>
{
	T operator()(const T& x, const T& y)const { return x * y; }
};

template<class T>
struct divides : public binary_function<T, T, T>
{
	T operator()(const T& x, const T& y)const { return x / y; }
};

template<class T>
struct modulus : public binary_function<T, T, T>
{
	T operator()(const T& x, const T& y)const { return x % y; }
};

template<class T>
struct negate : public unary_fuction<T, T>
{
	T operator()(const T& x)const { return -x; }
};

//���� ������ ���� ���ڵ��� С�� С�ڵ���
template<class T>
struct equal_to : public binary_function<T, T, bool>
{
	bool operator()(const T& x, const T& y)const { return x == y; }
};

template<class T>
struct not_equal_to : public binary_function<T, T, bool>
{
	bool operator()(const T& x, const T& y)const { return x != y; }
};

template<class T>
struct greater : public binary_function<T, T, bool>
{
	bool operator()(const T& x, const T& y)const { return x > y; }
};

template<class T>
struct less : public binary_function<T, T, bool>
{
	bool operator()(const T& x, const T& y)const { return x < y; }
};

template<class T>
struct greater_equal : public binary_function<T, T, bool>
{
	bool operator()(const T& x, const T& y)const { return x >= y; }
};

template<class T>
struct less_equal : public binary_function<T, T, bool>
{
	bool operator()(const T& x, const T& y)const { return x <= y; }
};


//�� �� ��
template<class T>
struct logical_and : public binary_function<T, T, bool>
{
	bool operator()(const T& x, const T& y)const { return x && y; }
};

template<class T>
struct logical_or : public binary_function<T, T, bool>
{
	bool operator()(const T& x, const T& y)const { return x || y; }
};

template<class T>
struct logical_not : public unary_fuction<T, bool>
{
	bool operator()(const T& x)const { return !x; }
};

//֤ͬ�� ѡ�� Ͷ��
template<class T>
struct identity : public unary_fuction<T, T>
{
	const T& operator()(const T& x)const { return x; }
};

template<class Pair>
struct select1st :public unary_fuction<Pair, typename Pair::first_type>
{
	const typename Pair::first_type& operator()(const Pair& x)const
	{
		return x.first;
	}
};

template<class Pair>
struct select2st :public unary_fuction<Pair, typename Pair::second_type>
{
	const typename Pair::first_type& operator()(const Pair& x)const
	{
		return x.second;
	}
};

template<class Arg1, class Arg2>
struct project1st : public binary_function<Arg1, Arg2, Arg1>
{
	Arg1 operator()(const Arg1& x, const Arg2&)const { return x; }
};

template<class Arg1, class Arg2>
struct project2st : public binary_function<Arg1, Arg2, Arg2>
{
	Arg2 operator()(const Arg1&, const Arg2& y)const { return y; }
};