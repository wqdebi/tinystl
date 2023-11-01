#pragma once
#include<new.h>

//��ָ��p������һ��T2���͵�value����
template<typename T1, typename T2>
inline void construct(T1* p, const T2& value)
{
	new(p)T1(value);
}

//���õ������������
template<typename T>
inline void destroy(T* pointer)
{
	pointer -> ~T();
}

//destroy�ڶ����汾�������������������������еĶ���
//��������ȡ����
template<typename ForwardIterator>
inline void destroy(ForwardIterator first, ForwardIterator last)
{
	__destory(first, last, value_type(first));
}

template<typename ForwardIterator, typename T>
inline void __destory(ForwardIterator first, ForwardIterator last, T*)
{
	typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;
	__destory_aux(first, last, trivial_destructor());
}

struct __false_type {};//��ʱ����Ҫ���ں���д��������ʱ���ھ���д��
struct __true_type {};//�Ժ���Ҫע�������д��룬�����д���ʹ����ȡ���ƺͺ������أ�ֱ�ӵ�����Ӧ�ĺ�����

template<class ForwardIterator>
inline void __destory_aux(ForwardIterator first, ForwardIterator last, __false_type)
{
	for (; first < last; ++first)
		destroy(&*first);
}

//ʲôҲ����
template<class ForwardIterator>
inline void __destory_aux(ForwardIterator first, ForwardIterator last, __true_type) {}

//�ػ���
inline void destroy(char*, char*) {}
inline void destroy(wchar_t*, wchar_t*) {}