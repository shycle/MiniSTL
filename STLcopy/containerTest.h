#pragma once
#include "s_function.h"
#include "s_vector.h"
#include "s_list.h"
#include "s_deque.h"
#include "s_stack.h"
#include "s_queue.h"
#include "s_heap.h"
#include "s_priority_queue.h"
#include "rb_tree.h"
#include "s_set.h"
#include "s_map.h"
#include "s_hashtable.h"
#include <iostream>
using namespace sgistl;
//using namespace std;

/*
 * vector 的操作
 */
template <class T>
void printVector(const vector<T>& vec)
{
	for (auto x : vec) {
		std::cout << x << " ";
	}

	std::cout <<std::endl;
}
#if 1 //vector Test
void vectorTest()
{
	int i;
	vector<int> iv(2, 9);
	std::cout << "size=" << iv.size() <<std::endl;
	std::cout << "capacity=" << iv.capacity() <<std::endl;

	iv.push_back(1);
	std::cout << "size=" << iv.size() <<std::endl;
	std::cout << "capacity=" << iv.capacity() <<std::endl;

	iv.push_back(2);
	std::cout << "size=" << iv.size() <<std::endl;
	std::cout << "capacity=" << iv.capacity() <<std::endl;

	iv.push_back(3);
	std::cout << "size=" << iv.size() <<std::endl;
	std::cout << "capacity=" << iv.capacity() <<std::endl;

	iv.push_back(4);
	std::cout << "size=" << iv.size() <<std::endl;
	std::cout << "capacity=" << iv.capacity() <<std::endl;

	for (i = 0; i < iv.size(); ++i)
	{
		std::cout << iv[i] << ' ';
	}
	std::cout <<std::endl;

	iv.push_back(5);
	std::cout << "size=" << iv.size() <<std::endl;
	std::cout << "capacity=" << iv.capacity() <<std::endl;
	for (i = 0; i < iv.size(); ++i)
	{
		std::cout << iv[i] << ' ';
	}
	std::cout <<std::endl;

	iv.pop_back();
	iv.pop_back();
	std::cout << "size=" << iv.size() <<std::endl;
	std::cout << "capacity=" << iv.capacity() <<std::endl;

	iv.pop_back();
	std::cout << "size=" << iv.size() <<std::endl;
	std::cout << "capacity=" << iv.capacity() <<std::endl;



	vector<int>::iterator ivite = std::find(iv.begin(), iv.end(), 1);
	if (ivite != iv.end())
		iv.erase(ivite);
	std::cout << "size=" << iv.size() <<std::endl;
	std::cout << "capacity=" << iv.capacity() <<std::endl;
	for (i = 0; i < iv.size(); ++i)
	{
		std::cout << iv[i] << ' ';
	}
	std::cout <<std::endl;
#	if 1
	ivite = std::find(iv.begin(), iv.end(), 2);
	if (ivite != iv.end())
		iv.insert(ivite, 3, 7);
	std::cout << "size=" << iv.size() <<std::endl;
	std::cout << "capacity=" << iv.capacity() <<std::endl;
	for (i = 0; i < iv.size(); ++i)
	{
		std::cout << iv[i] << ' ';
	}
	std::cout <<std::endl;

	vector<int> vec2 = vector<int>(iv.begin(), iv.end());
	std::cout << "vec2=" << vec2.capacity() <<std::endl;
	for (i = 0; i < vec2.size(); ++i)
	{
		std::cout << vec2[i] << ' ';
	}

	iv.clear();
	std::cout << "size=" << iv.size() <<std::endl;
	std::cout << "capacity=" << iv.capacity() <<std::endl;
#endif
}

void vectorTest2()
{
	//sgistl::vector<int> x;
	sgistl::vector<int> v(10, 5);
	std::cout << "v:" << v.size() << std::endl;
	v.push_back(3);
	v.push_back(19);
	std::cout << "v:" << v.size() << std::endl;
	for (auto x : v)
	{
		std::cout << x << " ";
	}
	auto it = v.begin();
	it = it + 10;
	v.erase(it);
	v.push_back(13);

	std::cout << "v11:" << v[11] << std::endl;

	std::cout << "v:" << v.size() << std::endl;

	//resize 测试
	v.resize(5,6);

	for (auto x : v)
	{
		std::cout << x << " ";
	}
	std::cout << std::endl;
//	sgistl::vector<std::string> vec(10, "haha");


}

//vector 内存配置检查
void vectorTest3()
{
	int i = 0;
	int sum = 1;
	sgistl::vector<int> vec;
	while (i < 10)
	{
		sum = sum + 14;//sum * 2;
		for (int j = 0; j < sum; j++)
		{
			vec.push_back(i);
		}
		std::cout << "sum:"<<sum<<"vec size:" << vec.size() << " vec cap:" << vec.capacity() << std::endl;
		i++;
	}
}

void vectorTest4()
{
#if 0
	vector<std::string> iv(6, "hi");
	printVector(iv);

	vector<std::string> sv;// {"why", "always", "me"};
	sv.push_back("why"); sv.push_back("always"); sv.push_back("me");
	printVector(sv);

	//vector<std::string> sv2(sv.begin(), sv.end());
	//	printVector(sv2);

	vector<std::string> sv3(sv);
	printVector(sv3);

	// operator= 赋值运算符
	vector<std::string> sv4;
	sv4 = sv;
	printVector(sv4);

	// assign
//	vector<char> cv;
//	cv.assign(5, 'x');
//	printVector(cv);

	//	vector<char> cv1;
	//	cv1.assign(cv.begin(), cv.end());
	//	printVector(cv1);

		// at(索引)
	//	std::cout << cv1.at(1) <<std::endl;

		// operator[] 
	//	cv1[1] = 'a';
	//	printVector(cv1);

		// front 容器首元素
	std::cout << sv.front() <<std::endl;

	// back 容器最后一个元素
	std::cout << sv.back() <<std::endl;

	// begin 返回指向容器第一个元素的迭代器
	// end 返回指向容器尾端的迭代器
	for (vector<std::string>::iterator it = sv.begin(); it != sv.end(); it++) {
		std::cout << *it << " ";
	}
	std::cout <<std::endl;

	// rbegin 返回一个指向容器最后一个元素的反向迭代器 
	// rend 返回一个指向容器前端的反向迭代器
//	for (vector<std::string>::reverse_iterator it = sv.rbegin(); it != sv.rend(); it++) {
//		std::cout << *it << " ";
//	}
//	std::cout <<std::endl;

	// empty 若容器为空则为 true ，否则为 false
	if (sv.empty()) {
		std::cout << "container is null." <<std::endl;
	}
	else {
		std::cout << "container is not null." <<std::endl;
	}

	// size	容器中的元素个数
	std::cout << sv.size() <<std::endl;

	// max_size 元素数量的最大值
	std::cout << sv.max_size() <<std::endl;

	// capacity 当前分配存储的容量
	std::cout << sv.capacity() <<std::endl;

	// resize 改变容器中可存储元素的个数 
	sv.resize(10);
	std::cout << sv.capacity() <<std::endl;

	// shrink_to_fit 请求移除未使用的容量
//	sv.shrink_to_fit();
	std::cout << sv.capacity() <<std::endl;

	// clear 从容器移除所有元素
	iv.clear();
	printVector(iv);

	// insert:三种形式
	auto it = sv.begin();
	it = sv.insert(it, "YES");
	printVector(sv);

	sv.insert(it, 2, "NO");
	printVector(sv);

	it = sv.begin();
	vector<std::string> sv5(2, "xx");
	//	sv.insert(it + 2, sv5.begin(), sv5.end());
	printVector(sv);

	// erase 从容器移除指定的元素
	sv.erase(sv.begin());
	printVector(sv);

	sv.erase(sv.begin() + 2, sv.begin() + 4);
	printVector(sv);

	// push_back 向容器尾部插入元素
	std::cout << sv.size() <<std::endl;
	sv.push_back("add");
	printVector(sv);

	// pop_back 移除容器的最末元素
	sv.pop_back();
	printVector(sv);

	// swap
	sv.swap(sv5);
	printVector(sv);
	printVector(sv5);
#endif
}

#endif

//list test
#if 1
void listTest()
{
	int i = 0;
	list<int> ilist;
	std::cout << "size=" << ilist.size() <<std::endl;

	ilist.push_back(0);
	ilist.push_back(1);
	ilist.push_back(2);
	ilist.push_back(3);
	ilist.push_back(4);
	ilist.push_back(5);

	std::cout << "size=" << ilist.size() <<std::endl;

	list<int>::iterator ite;
	for (ite = ilist.begin(); ite!= ilist.end(); ite++)
	{
		std::cout << *ite << ' ';
	}
	std::cout <<std::endl;

	ite = std::find(ilist.begin(), ilist.end(), 3);
	if (ite != 0)
	{
		ilist.insert(ite, 99);
	}
	std::cout << "size=" << ilist.size() <<std::endl;
	std::cout << *ite <<std::endl;

	for (ite = ilist.begin(); ite != ilist.end(); ite++)
	{
		std::cout << *ite << ' ';
	}
	std::cout <<std::endl;

	ite = std::find(ilist.begin(), ilist.end(), 1);
	if (ite != 0)
	{
		std::cout << *(ilist.erase(ite)) <<std::endl;
	}

	for (ite = ilist.begin(); ite != ilist.end(); ite++)
		std::cout << *ite << ' ';	
	std::cout <<std::endl;

}

void listTest2()
{
	list<int> ilist;
	ilist.push_back(0);
	ilist.push_back(2);
	ilist.push_back(99);
	ilist.push_back(3);
	ilist.push_back(4);
	
	list<int> ilist2;
	ilist.push_back(5);
	ilist.push_back(6);
	ilist.push_back(7);
	ilist.push_back(8);
	ilist.push_back(9);

	list<int>::iterator ite = std::find(ilist.begin(), ilist.end(), 99);
	ilist.splice(ite, ilist2);
	std::cout << "size=" << ilist.size() <<std::endl;
	for (ite = ilist.begin(); ite != ilist.end(); ite++)
		std::cout << *ite << ' ';
	std::cout <<std::endl;

	ilist.reverse();
	std::cout << "size=" << ilist.size() <<std::endl;
	for (ite = ilist.begin(); ite != ilist.end(); ite++)
		std::cout << *ite << ' ';
	std::cout <<std::endl;
	
	ilist.sort();
	std::cout << "size=" << ilist.size() <<std::endl;
	for (ite = ilist.begin(); ite != ilist.end(); ite++)
		std::cout << *ite << ' ';
	std::cout <<std::endl;
}
#endif

#if 1 //deque test
void dequeTest() {
	deque<int, alloc, 8> ideq(20, 9);
	std::cout << "size=" << ideq.size() <<std::endl;

	for (int i = 0; i < ideq.size(); ++i) {
		ideq[i] = i;
	}

	for (int i = 0; i < ideq.size(); ++i)
		std::cout << ideq[i] << ' ';
	std::cout <<std::endl;

	for (int i = 0; i < 3; i++)
		ideq.push_back(i);

	for (int i = 0; i < ideq.size(); ++i)
		std::cout << ideq[i] << ' ';
	std::cout <<std::endl;
	std::cout << "size=" << ideq.size() <<std::endl;

	ideq.push_back(3);
	for(int i=0;i<ideq.size();++i)
		std::cout << ideq[i] << ' ';
	std::cout <<std::endl;
	std::cout << "size=" << ideq.size() <<std::endl;

	ideq.push_front(99);
	for (int i = 0; i < ideq.size(); ++i)
		std::cout << ideq[i] << ' ';
	std::cout <<std::endl;
	std::cout << "size=" << ideq.size() <<std::endl;

	ideq.push_front(98);
	ideq.push_front(97);
	for (int i = 0; i < ideq.size(); ++i)
		std::cout << ideq[i] << ' ';
	std::cout <<std::endl;
	std::cout << "size=" << ideq.size() <<std::endl;

//	deque<int, alloc, 32>::iterator ite;
	auto ite = std::find(ideq.begin(), ideq.end(), 99);
//	ite = find(ideq.begin(), ideq.end(), 99);
	std::cout << *ite <<std::endl;
	std::cout << *(ite.cur) <<std::endl;
	ideq.erase(ite);

	for (int i = 0; i < ideq.size(); ++i)
		std::cout << ideq[i] << ' ';
	std::cout <<std::endl;
	std::cout << "size=" << ideq.size() <<std::endl;

	ideq.erase(std::find(ideq.begin(), ideq.end(), 17));
	ideq.push_back(39);
	ideq.push_front(74);
	for (int i = 0; i < ideq.size(); ++i)
		std::cout << ideq[i] << ' ';
	std::cout <<std::endl;
	std::cout << "size=" << ideq.size() <<std::endl;

	ideq.pop_front();
	ideq.pop_front();
	ideq.pop_back();
	ideq.pop_back();

	for (int i = 0; i < ideq.size(); ++i)
		std::cout << ideq[i] << ' ';
	std::cout <<std::endl;
	std::cout << "size=" << ideq.size() <<std::endl;

}

void stackTest()
{
	stack<int> s;
	s.push(3);
	s.push(7);
	s.push(-33);
	s.push(39);
	s.push(73);
	std::cout << s.size() << std::endl;


	s.pop();
	s.pop();

	std::cout << "top:" << s.top() << std::endl;
	std::cout << s.size() << std::endl;
}

void stackTest2()
{
	stack<int, list<int>> istack;
	istack.push(1);
	istack.push(3);
	istack.push(5);
	istack.push(7);

	std::cout << istack.size() << std::endl;
	std::cout << istack.top() << std::endl;


	istack.pop();	std::cout << istack.top() << std::endl;
	istack.pop();	std::cout << istack.top() << std::endl;
	istack.pop();	std::cout << istack.top() << std::endl;

	std::cout << istack.size() << std::endl;
}

void queueTest()
{
	queue<int, list<int>> iqueue;
	iqueue.push(1);
	iqueue.push(3);
	iqueue.push(5);
	iqueue.push(7);

	std::cout << iqueue.size() << std::endl;
	std::cout << iqueue.front() << std::endl;


	iqueue.pop();	std::cout << iqueue.front() << std::endl;
	iqueue.pop();	std::cout << iqueue.front() << std::endl;
	iqueue.pop();	std::cout << iqueue.front() << std::endl;

	std::cout << iqueue.size() << std::endl;
}

void heapTest()
{
	int ia[9] = { 0,1,2,3,4,8,9,3,5 };
	vector<int> ivec(ia, ia + 9);
	make_heap(ivec.begin(), ivec.end());
	for (int i = 0; i < ivec.size(); ++i)
		std::cout << ivec[i] << ' ';
	std::cout <<std::endl;

	ivec.push_back(7);
	push_heap(ivec.begin(), ivec.end());
	for (int i = 0; i < ivec.size(); ++i)
		std::cout << ivec[i] << ' ';

	pop_heap(ivec.begin(), ivec.end());
	std::cout << ivec.back() <<std::endl;
	ivec.pop_back();

	for (int i = 0; i < ivec.size(); ++i)
		std::cout << ivec[i] << ' ';
	std::cout <<std::endl;

	sort_heap(ivec.begin(), ivec.end());
	for (int i = 0; i < ivec.size(); ++i)
		std::cout << ivec[i] << ' ';
	std::cout <<std::endl;

}

void priority_queueTest() {
	int ia[9] = { 0, 1, 2, 3, 4, 8, 9, 3, 5 };
	priority_queue<int> ipq(ia, ia + 9);
	std::cout << "size=" << ipq.size() <<std::endl;

	for (int i = 0; i < ipq.size(); ++i)
		std::cout << ipq.top() << ' ';
	std::cout <<std::endl;

	while (!ipq.empty()) {
		std::cout << ipq.top() << ' ';
		ipq.pop();
	}
	std::cout <<std::endl;
}

// 定义二元函数的参数型别的返回值型别
template <class Arg1, class Arg2, class Result>
struct binary_functiona
{
	typedef Arg1      first_argument_type;
	typedef Arg2      second_argument_type;
	typedef Result    result_type;
};

// 函数对象：小于
template <class T>
struct lessa :public binary_functiona<T, T, bool>
{
	bool operator()(const T& x, const T& y) const { return x < y; }
};

void rb_treeTest() {
	rb_tree<int, int, identity<int>, lessa<int>> itree;
	std::cout << itree.size() <<std::endl;

	itree.insert_unique(10);
	itree.insert_unique(7);
	itree.insert_equal(19);
	itree.insert_unique(8);
	itree.insert_unique(15);
	itree.insert_unique(5);
	itree.insert_unique(6);
	itree.insert_unique(11);
	itree.insert_unique(13);
	itree.insert_unique(12);
	std::cout << itree.size() <<std::endl;
	auto iter1 = itree.begin();
	auto iter2 = itree.end();
	__rb_tree_base_iterator rbtite;
	for (int i = 0; i < 9; ++iter1, ++i)
	{
		rbtite = __rb_tree_base_iterator(iter1);
		std::cout << *iter1 << '(' << rbtite.node->color << ')';
	}
	std::cout <<std::endl;
}


void setTest()
{
	int i = 0;
	int ia[5] = { 0,1,2,3,4 };
//	rb_tree<int, int, identity<int>, lessa<int>> itree;
	set<int> iset(ia, ia + 5);



#if 1
	std::cout << "size=" << iset.size() <<std::endl;
	std::cout << "3 count=" << iset.count(3) <<std::endl;
	iset.insert(3);

	std::cout << "size=" << iset.size() <<std::endl;
	std::cout << "3 count=" << iset.count(3) <<std::endl;
	iset.insert(5);

	std::cout << "size=" << iset.size() <<std::endl;
	std::cout << "3 count=" << iset.count(3) <<std::endl;
//	iset.erase(1);
	std::cout << "size=" << iset.size() <<std::endl;
	std::cout << "3 count=" << iset.count(3) <<std::endl;
	std::cout << "1 count=" << iset.count(1) <<std::endl;

	set<int>::iterator iter1 = iset.begin();
	auto iter2 = iset.end();
	for (; iter1 != iter2; ++iter1)
		std::cout << *iter1;
	std::cout <<std::endl;

	iter1 = std::find(iset.begin(), iset.end(), 3);
	if (iter1 != iset.end())
		std::cout << "3 found" <<std::endl;

	iter1 = std::find(iset.begin(), iset.end(), 7);
	if (iter1 == iset.end())
		std::cout << "7 not found" <<std::endl;

	iter1 = iset.find(3);
	if (iter1 != iset.end())
		std::cout << "3 found" <<std::endl;

	iter1 = iset.find(1);
	if (iter1 == iset.end())
		std::cout << "1 not found" <<std::endl;
#endif
}

void mapTest() {
	map<std::string, int> ss;
	ss.test();
#if 0
	pair<std::string, int> value(std::string("david"), 5);
	map<std::string, int> simap;
	simap[std::string("jjhou")] = 1;
	simap[std::string("jerry")] = 2;
	simap[std::string("jason")] = 3;
	simap[std::string("jimmy")] = 4;

	simap.insert(value);

	map<std::string, int>::iterator simap_iter = simap.begin();
	for (; simap_iter != simap.end(); ++simap_iter)
		std::cout << simap_iter->first << ' ' << simap_iter->second << std::endl;
	int number = simap[std::string("jjhou")];
	std::cout << number << std::endl;

	auto  ite1 = simap.find(std::string("mchen"));
	if (ite1 == simap.end())
		std::cout << "mchen not found" << std::endl;
	ite1 = simap.find(std::string("jerry"));
	if (ite1 != simap.end())
		std::cout << "jerry found" << std::endl;
	ite1->second = 9;
	int number2 = simap[("jerry")];
	std::cout << number2 << std::endl;
#endif
}
#endif


void HashTableTest() {
	hashtable<int, int, std::hash<int>, identity<int>, std::equal_to<int>, alloc> iht(50, std::hash<int>(), std::equal_to<int>());
	std::cout<<iht.size()<<std::endl;
	std::cout<<iht.bucket_count() <<std::endl;

	iht.insert_unique(59);
	iht.insert_unique(63);
	iht.insert_unique(108);
	iht.insert_unique(2);
	iht.insert_unique(53);
	iht.insert_unique(55);
	std::cout << iht.size() << std::endl;
	std::cout << iht.bucket_count() << std::endl;

	hashtable<int, int, std::hash<int>, identity<int>, std::equal_to<int>, alloc>::iterator ite = iht.begin();

	for (int i = 0; i < iht.size(); i++, ++ite)
		std::cout << *ite << ' ';
	std::cout << std::endl;

	for (int i = 0; i < iht.bucket_count(); ++i) {
		int n = iht.elems_in_bucket(i);
		if (n != 0)
			std::cout << "bucket[" << i << "]has " << n << " elems. " << std::endl;
	}

	//验证bucket容量是否就是buckets vector的大小
	for (int i = 0; i < 51; i++)
		iht.insert_equal(i);
	std::cout << iht.size() << std::endl;
	std::cout << iht.bucket_count() << std::endl;

	for (int i = 0; i < iht.bucket_count(); ++i)
	{
		int  n = iht.elems_in_bucket(i);
		if (n != 0)
			std::cout << "bucket[" << i << "] has" << n << " elems." << std::endl;
	}


	ite = iht.begin();
	for (int i = 0; i < iht.size(); i++, ++ite)
		std::cout << *ite << ' ';
	std::cout << std::endl;

	std::cout << *(iht.find(2)) << std::endl;
	std::cout << iht.count(2) << std::endl;
}

void HashSet() {
	//hash_set<const char*, hash<const char*>, eqstr> Set;
}