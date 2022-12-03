#pragma once
#include "s_iterator.h"
#include "s_alloc.h"
#include "stl_config.h"
#include "s_pair.h"
using namespace sgistl;

typedef bool __rb_tree_color_type;
const __rb_tree_color_type __rb_tree_red = false;
const __rb_tree_color_type __rb_tree_black = true;

struct __rb_tree_node_base {
	typedef __rb_tree_color_type color_type;
	typedef __rb_tree_node_base* base_ptr;

	color_type color;
	base_ptr parent;
	base_ptr left;
	base_ptr right;

	static base_ptr minimum(base_ptr x) {
		while (x->left != 0) x = x->left;
		return x;
	}

	static base_ptr maximum(base_ptr x) {
		while (x->right != 0) x = x->right;
		return x;
	}
};

template <class Value>
struct __rb_tree_node :public __rb_tree_node_base {
	typedef __rb_tree_node<Value>* link_type;
	Value value_field;

	link_type get_node_ptr()
	{
		return &*this;
	}
};

struct __rb_tree_base_iterator {
	typedef __rb_tree_node_base::base_ptr base_ptr;
	typedef sgistl::bidirectional_iterator_tag iterator_category;
	typedef ptrdiff_t difference_type;

	base_ptr node;
	void increment() {
		if (node->right != 0) {
			node = node->right;
			while (node->left != 0)
				node = node->left;
		}
		else
		{
			base_ptr y = node->parent;
			while (node == y->right) {
				node = y;
				y = y->parent;
			}
			if (node->right != y)
				node = y;
		}
	}

	void decrement() {
		//如果是红节点且父节点的父节点等于自己   1：右子节点为答案，此情况发生于node为header时（亦即node为end()时）
		if (node->color == __rb_tree_red && node->parent->parent == node)
			node = node->right;
		else if (node -> left != 0) {
			//2：如果有左子节点，令y指向左节点， 当y有右子节点时，一直往右子节点走到底，最后即使答案 
			base_ptr y = node->left;
			while (y->right != 0)
				y = y->right;
			node = y;
		}
		else {
			//3：非根节点，也无左子节点 找出夫节点，当现行节点身为左子节点 一直交替往上走，直到现行节点不为左子节点
			base_ptr y = node->parent;
			while (node == y->left) {
				node = y;
				y = y->parent;
			}
			node = y;
		}
	}
};

template <class Value, class Ref, class Ptr>
struct __rb_tree_iterator :public __rb_tree_base_iterator {
	typedef Value value_type;
	typedef Ptr pointer;
	typedef Ref reference;
	typedef __rb_tree_iterator<Value, Value&, Value*>	iterator;
	typedef __rb_tree_iterator<Value, const Value&, const Value*>	const_iterator;
	typedef __rb_tree_iterator<Value, Ref, Ptr>	self;
	typedef __rb_tree_node<Value>* link_type;

	__rb_tree_iterator() { }
	__rb_tree_iterator(link_type x) { node = x; }
	__rb_tree_iterator(const_iterator& it) { node = it.node; }

	reference operator*() { return link_type(node)->value_field; }
	pointer operator->() { return &(operator*()); }
	self& operator++() { increment(); return *this; }
	self operator++(int) {
		self tmp = *this;
		increment();
		return tmp;
	}
	self& operator--() { decrement();  return *this; }
	self operator--(int) {
		self tmp = *this;
		decrement();
		return tmp;
	}
	//bool operator==(){ return }
};

inline bool operator==(const __rb_tree_base_iterator& __x,
	const __rb_tree_base_iterator& __y) {
	return __x.node == __y.node;
}

inline bool operator!=(const __rb_tree_base_iterator& __x,
	const __rb_tree_base_iterator& __y) {
	return __x.node != __y.node;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc = alloc>
class rb_tree {
protected:
	typedef void* void_pointer;
	typedef __rb_tree_node_base* base_ptr;
	typedef __rb_tree_node<Value> rb_tree_node;
	typedef simple_alloc<rb_tree_node, Alloc> rb_tree_node_allocator;
	typedef __rb_tree_color_type color_type;
public:
	typedef Key key_type;
	typedef Value value_type;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef rb_tree_node* link_type;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

public:
//	rb_tree() :node_count(0), key_compare() { init(); }
	rb_tree(const Compare& comp = Compare()) :node_count(0), key_compare(comp), header(0) {
		init();
	}

	explicit rb_tree(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& __x)
		: node_count(0), key_compare(__x.key_compare), header(0)
	{
		if (__x.root() == 0)
			init();
		else {
			color(header) = __rb_tree_red;
			root() = copy(__x.root(), header);
			leftmost() = minimum(root());
			rightmost() = maximum(root());
		}
		node_count = __x.node_count;
	}


	~rb_tree() {
		clear();
		put_node(header);
	}


protected:
	link_type get_node() { return rb_tree_node_allocator::allocate(); }
	void put_node(link_type p) { rb_tree_node_allocator::deallocate(p); }

	link_type create_node(const value_type& x) {
		link_type tmp = get_node();
		__STL_TRY{
			construct(&tmp->value_field, x);
		}
		__STL_UNWIND(put_node(tmp));
		return tmp;
	}

	link_type clone_node(link_type x) {
		link_type tmp = create_node(x->value_filed);
		tmp->color = x->color;
		tmp->left = 0;
		tmp->right = 0;
		return tmp;
	}

	void destroy_node(link_type p) {
		destroy(&p->value_field);
		put_node(p);
	}

protected:
	size_type node_count;
	link_type header;
	Compare key_compare;

	//方便取得header成员的三个函数
	link_type& root() const { return (link_type&)header->parent; }
	link_type& leftmost() const { return (link_type&)header->left; }
	link_type& rightmost() const { return (link_type&)header->right; }

	//方便去的节点x的成员
	static link_type& left(link_type x)
	{
		return (link_type&)(x->left);
	}
	static link_type& right(link_type x)
	{
		return (link_type&)(x->right);
	}
	static link_type& parent(link_type x)
	{
		return (link_type&)(x->parent);
	}
	static reference value(link_type x)
	{
		return x->value_field;
	//	return (link_type&)(x->value_field);
	}
	static const Key& key(link_type x)
	{
		return KeyOfValue()(value(x));
	}
	static color_type& color(link_type x)
	{
		return (color_type&)(x->color);
	}

	static link_type& left(base_ptr x)
	{
		return (link_type&)(x->left);
	}
	static link_type& right(base_ptr x)
	{
		return (link_type&)(x->right);
	}
	static link_type& parent(base_ptr x)
	{
		return (link_type&)(x->parent);
	}
	static reference value(base_ptr x)
	{
		return ((link_type)x)->value_field;
	}
	static const Key& key(base_ptr x)
	{
		return KeyOfValue()(value(link_type(x)));
	}
	static color_type& color(base_ptr x)
	{
		return (color_type&)(link_type(x)->color);
	}

	static link_type minimum(link_type x) {
		return (link_type)__rb_tree_node_base::minimum(x);
	}

	static link_type maximum(link_type x) {
		return (link_type)__rb_tree_node_base::maximum(x);
	}

public:
	typedef __rb_tree_iterator<value_type, reference, pointer> iterator;
//	typedef __rb_tree_iterator<value_type, reference, pointer> reverse_iterator;
	typedef __rb_tree_iterator<value_type, const_reference, const_pointer> const_iterator;

private:
	iterator __insert(base_ptr x, base_ptr y, const value_type& v);
	link_type __copy(link_type x, link_type p);
	void __erase(link_type x) {
		// erase without rebalancing
		while (x != 0) {
			__erase(right(x));
			link_type y = left(x);
			destroy_node(x);
			x = y;
		}
	}
	void init() {
		header = get_node(); //产生一个节点空间， 令header为红色， 用来区分header和root
		header->color = __rb_tree_red;
		root() = 0;
		leftmost() = header;
		rightmost() = header;
	}
	void clear() {
		//?? 如何在红黑树中删除所有的节点
		//link_type cur = header;
		if (node_count != 0) {
			__erase(root());
			leftmost() = header;
			root() = 0;
			rightmost() = header;
			node_count = 0;
		}
	}

public:
	rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& operator=(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x)
	{

	}

public:
	Compare key_comp() const { return key_compare; }
	iterator begin() { return leftmost(); }
	iterator end() {
		return header;
	}//rightmost(); }
//	reverse_iterator begin() { return rightmost(); }
//	reverse_iterator end() { return leftmost(); }
	bool empty() const { return node_count; }
	size_type size() const { return node_count; }
	size_type max_size() const { return size_type(-1); }
	iterator find(const key_type& v);
//	size_type count(const value_type& v) const;
	size_type count(const key_type& v);

	iterator lower_bound(const key_type& x);
	iterator upper_bound(const key_type& x);
	const_iterator lower_bound(const key_type& x) const;
	const_iterator upper_bound(const key_type& x) const;
	pair<iterator, iterator> equal_range(const key_type& __k);
	pair<const_iterator, const_iterator> equal_rangec(const key_type& __k) const;


public:
	//插入节点到红黑树，保证节点值唯一
	pair<iterator, bool> insert_unique(const value_type& v);

	iterator insert_unique(iterator pos, const value_type& v);

//	void insert_unique(const_iterator first, const_iterator last) {
//		for (; first != last; ++first)
//			insert_unique(*first);
//	}

	template <class InputIterator>
	void insert_unique(InputIterator first, InputIterator last) {
		for (; first != last; ++first)
			insert_unique(*first);
	}

	//允许节点值重复的插入
	iterator insert_equal(const value_type& v) {
		link_type y = header;
		link_type x = root();
		while (x != 0) {
			y = x;
			x = key_compare(KeyOfValue()(v), key(x)) ? left(x) : right(x);
			//待插入的值， 遇 大 则往左 遇小则往右
		}
		return __insert(x, y, v);
		//x为新值插入点， y为插入点的父节点， v为新值
	}

	size_type erase(const key_type& x) {
		
		pair<iterator, iterator> __p = equal_range(x);
		size_type __n = 0;
		distance(__p.first, __p.second, __n);
		erase(__p.first, __p.second);
		return __n;
	}

	size_type erase_unique(const key_type& key)
	{
		auto it = find(key);
		if (it != end())
		{
			erase_u(it);
			return 1;
		}
		return 0;
	}

	iterator erase_u(iterator hint)
	{
		auto node = &*hint.node;
		auto next = hint++;

	//	rb_tree_erase_rebalance(hint.node, root(), leftmost(), rightmost());
		auto x = (link_type)rb_tree_rebalance_for_erase(hint.node,
			header->parent,
			header->left,
			header->right);
	//	destroy_node(node);
		--node_count;
		return next;
	}

	inline 
	void erase(iterator pos) {
		link_type y =
			(link_type)rb_tree_rebalance_for_erase(pos.node, root(), leftmost(), rightmost());
		destroy_node(y);
		--node_count;
	}

	void erase(iterator first, iterator last) {
		if (first == begin() && last == end())
			clear();
		else
			while (first != last) erase(first++);
	}
};

inline void
_rb_tree_rotate_left(__rb_tree_node_base* __x, __rb_tree_node_base*& __root)
{
	__rb_tree_node_base* __y = __x->right;
	__x->right = __y->left;
	if (__y->left != 0)
		__y->left->parent = __x;
	__y->parent = __x->parent;

	if (__x == __root)
		__root = __y;
	else if (__x == __x->parent->left)
		__x->parent->left = __y;
	else
		__x->parent->right = __y;
	__y->left = __x;
	__x->parent = __y;
}

inline void
_rb_tree_rotate_right(__rb_tree_node_base* __x, __rb_tree_node_base*& __root)
{
	__rb_tree_node_base* __y = __x->left;
	__x->left = __y->right;
	if (__y->right != 0)
		__y->right->parent = __x;
	__y->parent = __x->parent;

	if (__x == __root)
		__root = __y;
	else if (__x == __x->parent->right)
		__x->parent->right = __y;
	else
		__x->parent->left = __y;
	__y->right = __x;
	__x->parent = __y;
}

inline void
__rb_tree_rebalance(__rb_tree_node_base* x, __rb_tree_node_base*& root)
{
	//x->color = __rb_tree_red;	//新节点必为红
	x->color = __rb_tree_red;
	while (x != root && x->parent->color == __rb_tree_red) {
		if (x->parent == x->parent->parent->left) {
			__rb_tree_node_base* __y = x->parent->parent->right;
			if (__y && __y->color == __rb_tree_red) {
				x->parent->color = __rb_tree_black;
				__y->color = __rb_tree_black;
				x->parent->parent->color = __rb_tree_red;
				x = x->parent->parent;
			}
			else {
				if (x == x->parent->right) {
					x = x->parent;
					_rb_tree_rotate_left(x, root);
				}
				x->parent->color = __rb_tree_black;
				x->parent->parent->color = __rb_tree_red;
				_rb_tree_rotate_right(x->parent->parent, root);
			}
		}
		else {
			__rb_tree_node_base* __y = x->parent->parent->left;
			if (__y && __y->color == __rb_tree_red) {
				x->parent->color = __rb_tree_black;
				__y->color = __rb_tree_black;
				x->parent->parent->color = __rb_tree_red;
				x = x->parent->parent;
			}
			else {
				if (x == x->parent->left) {
					x = x->parent;
					_rb_tree_rotate_right(x, root);
				}
				x->parent->color = __rb_tree_black;
				x->parent->parent->color = __rb_tree_red;
				_rb_tree_rotate_left(x->parent->parent, root);
			}
		}
	}
	root->color = __rb_tree_black;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const Key& v)
{
	link_type __y = header;      // Last node which is not less than __k. 
	link_type __x = root();      // Current node. 

	while (__x != 0)
	{
		if (!key_compare(key(__x), v))
			__y = __x, __x = left(__x);
		else
			__x = right(__x);
	}
	iterator j = iterator(__y);
	return (j == end() || key_compare(v, key(j.node))) ? end() : j;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::size_type
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::count(const key_type& v) //const
{
//	pair<const_iterator, const_iterator> __p = equal_range(v);
	pair<iterator, iterator> __p = equal_range(v);
	size_type __n = 0;
	__n = __n + distance(__p.first, __p.second);
	return __n;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::lower_bound(const key_type& __k)
{
	link_type __y = header; /* Last node which is not less than __k. */
	link_type __x = root(); /* Current node. */

	while (__x != 0)
		if (!key_compare(key(__x), __k))
			__y = __x, __x = left(__x);
		else
			__x = right(__x);

	return iterator(__y);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::lower_bound(const key_type& __k) const
{
	link_type __y = header; /* Last node which is not less than __k. */
	link_type __x = root(); /* Current node. */

	while (__x != 0)
		if (!key_compare(key(__x), __k))
			__y = __x, __x = left(__x);
		else
			__x = right(__x);

	return const_iterator(__y);
}


template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::upper_bound(const key_type& __k)
{
	link_type __y = header; /* Last node which is not less than __k. */
	link_type __x = root(); /* Current node. */

	while (__x != 0)
		if (key_compare(__k, key(__x)))
			__y = __x, __x = left(__x);
		else
			__x = right(__x);

	return iterator(__y);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::upper_bound(const key_type& __k) const
{
	link_type __y = header; /* Last node which is not less than __k. */
	link_type __x = root(); /* Current node. */

	while (__x != 0)
		if (key_compare(__k, key(__x)))
			__y = __x, __x = left(__x);
		else
			__x = right(__x);

	return const_iterator(__y);
}


template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline
pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator, typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator>
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::equal_range(const key_type& __k)
{
	return pair<iterator, iterator>(lower_bound(__k), upper_bound(__k));
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline
pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator, typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator>
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::equal_rangec(const key_type& __k) const
{
	return pair<const_iterator, const_iterator>(lower_boundc(__k), upper_boundc(__k));
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__insert(base_ptr x_, base_ptr y_, const value_type& v) {
	link_type x = (link_type)x_;
	link_type y = (link_type)y_;
	link_type z;

	//key_compare是键值大小比较准则， 应该会是个function object
	if (y == header || x != 0 || key_compare(KeyOfValue()(v), key(y))) {
		z = create_node(v);
		left(y) = z;	//使得 当y即为header时， leftmost = z
		if (y == header) {
			root() = z;
			rightmost() = z;
		}
		else if (y == leftmost())  //如果y为最左节点
			leftmost() = z;  //维护leftmost， 使它永远指向最左节点
	}
	else {
		z = create_node(v);
		right(y) = z;	//令新节点z  成为插入节点的父节点y的右子节点
		if (y == rightmost())
			rightmost() = z;  //维护rightmost， 使它永远指向最右节点
	}
	parent(z) = y;//设新父
	left(z) = 0;
	right(z) = 0;
	//新节点的颜色将在__rb_tree_rebalance()设定（并调整）
	__rb_tree_rebalance(z, header->parent);//参数一为新增节点， 参数二为root
	++node_count;
	return iterator(z); //返回一个迭代器， 指向新增节点
}

/*
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(const value_type& v) {
	link_type y = header;
	link_type x = root();
	while (x != 0) {
		y = x;
		x = key_compare(KeyOfValue()(v), key(x)) ? left(x) : right(x);
		//待插入的值， 遇 大 则往左 遇小则往右
	}
	return __insert(x, y, v);
	//x为新值插入点， y为插入点的父节点， v为新值
}
*/

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator, bool> 
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(const value_type& v) {
	link_type y = header;
	link_type x = root();
	bool comp = true;
	while (x != 0) {
		y = x;
		comp = key_compare(KeyOfValue()(v), key(x));
		x = comp ? left(x) : right(x);
	}//循环结束后， y所指 即插入点的父节点（插入节点必为叶节点）

	iterator j = iterator(y);//令迭代器j指向插入点的父节点
	if (comp)
		if (j == begin())
			return pair<iterator, bool>(__insert(x, y, v), true);
		else
			--j;
	if (key_compare(key(j.node), KeyOfValue()(v)))
	//新建值不与既有节点之键值重复， 于是以下执行安插操作
		return pair<iterator, bool>(__insert(x, y, v), true);
	//以上 x为新值插入点， y为插入点的父节点， v为新值
	
	//行至此时， 表示新值一定与树中键值重读， 那么就不该插入新值。
	return pair<iterator, bool>(j, false);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator 
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(iterator pos, const value_type& __v) {
	if (pos.node == header->left) { // begin()
		if (size() > 0 &&
			key_compare(KeyOfValue()(__v), key(pos.node)))
			return insert(pos.node, pos.node, __v);
		// first argument just needs to be non-null 
		else
			return insert_unique(__v).first;
	}
	else if (pos.node == header) { // end()
		if (key_compare(key(rightmost()), KeyOfValue()(__v)))
			return insert(0, rightmost(), __v);
		else
			return insert_unique(__v).first;
	}
	else {
		iterator __before = pos;
		--__before;
		if (key_compare(key(__before.node), KeyOfValue()(__v))
			&& key_compare(KeyOfValue()(__v), key(pos.node))) {
			if (right(__before.node) == 0)
				return insert(0, __before.node, __v);
			else
				return insert(pos.node, pos.node, __v);
			// first argument just needs to be non-null 
		}
		else
			return insert_unique(__v).first;
	}
}

inline
__rb_tree_node_base* rb_tree_rebalance_for_erase(__rb_tree_node_base* z,
	__rb_tree_node_base*& __root,
	__rb_tree_node_base*& __leftmost,
	__rb_tree_node_base*& __rightmost)
{
	__rb_tree_node_base* y = z;
	__rb_tree_node_base* x = 0;
	__rb_tree_node_base* x_parent = 0;
	if (y->left == 0)     // z has at most one non-null child. y == z.
		x = y->right;     // x might be null.
	else
		if (y->right == 0)  // z has exactly one non-null child. y == z.
			x = y->left;    // x is not null.
		else {                   // z has two non-null children.  Set y to
			y = y->right;   //   z's successor.  x might be null.
			while (y->left != 0)
				y = y->left;
			x = y->right;
		}
	if (y != z) {          // relink y in place of z.  y is z's successor
		z->left->parent = y;
		y->left = z->left;
		if (y != z->right) {
			x_parent = y->parent;
			if (x) x->parent = y->parent;
			y->parent->left = x;      // y must be a child of left
			y->right = z->right;
			z->right->parent = y;
		}
		else
			x_parent = y;
		if (__root == z)
			__root = y;
		else if (z->parent->left == z)
			z->parent->left = y;
		else
			z->parent->right = y;
		y->parent = z->parent;
		__STD::swap(y->color, z->color);
		y = z;
		// y now points to node to be actually deleted
	}
	else {                        // y == z
		x_parent = y->parent;
		if (x) x->parent = y->parent;
		if (__root == z)
			__root = x;
		else
			if (z->parent->left == z)
				z->parent->left = x;
			else
				z->parent->right = x;
		if (__leftmost == z)
			if (z->right == 0)        // z->left must be null also
				__leftmost = z->parent;
		// makes __leftmost == header if z == __root
			else
				__leftmost = __rb_tree_node_base::minimum(x);
		if (__rightmost == z)
			if (z->left == 0)         // z->right must be null also
				__rightmost = z->parent;
		// makes __rightmost == header if z == __root
			else                      // x == z->left
				__rightmost = __rb_tree_node_base::maximum(x);
	}
	if (y->color != __rb_tree_red) {
		while (x != __root && (x == 0 || x->color == __rb_tree_black))
			if (x == x_parent->left) {
				__rb_tree_node_base* __w = x_parent->right;
				if (__w->color == __rb_tree_red) {
					__w->color = __rb_tree_black;
					x_parent->color = __rb_tree_red;
					_rb_tree_rotate_right(x_parent, __root);
					__w = x_parent->right;
				}
				if ((__w->left == 0 ||
					__w->left->color == __rb_tree_black) &&
					(__w->right == 0 ||
						__w->right->color == __rb_tree_black)) {
					__w->color = __rb_tree_red;
					x = x_parent;
					x_parent = x_parent->parent;
				}
				else {
					if (__w->right == 0 ||
						__w->right->color == __rb_tree_black) {
						if (__w->left) __w->left->color =__rb_tree_black;
						__w->color =__rb_tree_red;
						_rb_tree_rotate_right(__w, __root);
						__w = x_parent->right;
					}
					__w->color = x_parent->color;
					x_parent->color =__rb_tree_black;
					if (__w->right) __w->right->color =__rb_tree_black;
					_rb_tree_rotate_right(x_parent, __root);
					break;
				}
			}
			else {                  // same as above, with right <-> left.
				__rb_tree_node_base* __w = x_parent->left;
				if (__w->color ==__rb_tree_red) {
					__w->color =__rb_tree_black;
					x_parent->color =__rb_tree_red;
					_rb_tree_rotate_right(x_parent, __root);
					__w = x_parent->left;
				}
				if ((__w->right == 0 ||
					__w->right->color ==__rb_tree_black) &&
					(__w->left == 0 ||
						__w->left->color ==__rb_tree_black)) {
					__w->color =__rb_tree_red;
					x = x_parent;
					x_parent = x_parent->parent;
				}
				else {
					if (__w->left == 0 ||
						__w->left->color ==__rb_tree_black) {
						if (__w->right) __w->right->color =__rb_tree_black;
						__w->color =__rb_tree_red;
						_rb_tree_rotate_right(__w, __root);
						__w = x_parent->left;
					}
					__w->color = x_parent->color;
					x_parent->color =__rb_tree_black;
					if (__w->left) __w->left->color =__rb_tree_black;
					_rb_tree_rotate_right(x_parent, __root);
					break;
				}
			}
		if (x) x->color =__rb_tree_black;
	}
	return y;
}
