/*
Dirac CPP Tool
Category: Memory managing

github.com/yagaodirac

DCT_object is the very base class of the whole Dct memory managing tool set.
This class is not designed for directly using.
The key purpose is to make sure all the objects have virtual destructor function and a type() to query the type.


To do:
I'm not sure how slow could dynamic cast be, but I'm considering adding some clue or what to speed it up somehow.

*/

#pragma once

#include <algorithm>

class DCT_memory_managing_backend;

class DCT_object
{
	friend DCT_memory_managing_backend;
public:
	DCT_object() { 
		pending_remove = false; 
		active = true;
	};
	virtual ~DCT_object() {};

	virtual const char* type() { return "DCT_object"; };
	virtual void receive_tick(float delta_time) = 0;
	virtual void tick(float delta_time) = 0;
	
	//For some classes which don't allow to be removed, this function does nothing and return false;
	virtual bool remove() = 0;
	bool pending_remove;
protected:

	/*This function should only be invoked by DCT_memory_managing_tool_backend::clean.
	But this is Dct. I'm not gonna prevent you from calling it.
	Let me think...
	*/
	//virtual void __clean() = 0;

public:

	virtual bool canContainChild()const=0;
	bool active;
	/*
	To do
	Maybe something like:
	enum class base_to_level1_cast_clue
	{
		base,
		node,
		component,
		user define type 1,
		user define type 2,
		user define type 3,
	}
	Means, when dynamic casts, the pointer first reinterpret cast to the the type correspongding to the clue, and then dynamic cast. I'm not sure whether some of the searching overhead could be avoid in this way.

	*/
	
};




class DCT_node :public DCT_object
{
	friend DCT_memory_managing_backend;
public:
	DCT_node(){}
	virtual ~DCT_node() {};
	virtual const char* type() override{ return "DCT_node"; };

	/*only node has create()
	Can only create components and nodes.
	*/
	bool set_parent(DCT_node* in_parent)
	{
		if (pending_remove)return false;//check this first
		if (in_parent->pending_remove)return false;//check this first
		
		if (parent)
		{
			parent->children.erase(std::remove(parent->children.begin(), parent->children.end(), this), parent->children.end());
		}

		parent = in_parent;
		parent->children.push_back(this);
		return true;
	}

private:
	//This is designed only to be call from the DCT_memory_managing_backend instance.
	bool __set_parent_invoked_by_DCT_memory_managing_backend(DCT_memory_managing_backend* in)
	{
		backend = in;
		return true;
	}
public:

	template<typename T>
	T* create() 
	{
		if (pending_remove)return nullptr;//check this first

		T* new_obj = new T();
		new_obj->parent = this;
		this->children.push_back( new_obj );
		return new_obj;
	}

	//remove() marks the object as pending deleting. It doesnot release memory.
	//To really release memory, call try_clean() from parent node or wait the DCT_memory_managing_tool_backend instance to try_clean() try cleanning all the tree.
	bool remove() override
	{
		for (auto& iter : children)
		{
			iter->remove();
		}
		pending_remove = true;
		return true;
	}

	bool is_root()const
	{
		if (!parent && backend)
			return true;
		else 
			return false;
	}

	template<typename T>
	T* find()
	{
		if (pending_remove)return nullptr;//check this first

		for (auto iter = children.begin(); iter != children.end(); iter++)
		{
			T* temp = dynamic_cast<T*>(*iter);
			if (temp)return temp;
		}
		return nullptr;
	};

	virtual bool canContainChild()const override{ return true; };

	virtual void receive_tick(float delta_time) override
	{//In this way you could control the sequency of ticks, node first or components first.
		for (auto element : children)
		{
			element->receive_tick(delta_time);
		}
		//I prefer to tick inside first.
		tick(delta_time);
	};
	//virtual void tick(float delta_time)override {};//I shouldn't override this.

	//The remove() doesnot actually delete anything or release any memory. Call try_clean() from parent node to release memory.
	void try_clean()
	{
		for(size_t i =0;i< children.size();/*manully control it*/)
		//for (std::vector<DCT_object*>::iterator iter = children.begin();iter!=children.end();/*manully control it*/)
		{
			if (children[i]->canContainChild())
			{//for DCT node
				if (children[i]->pending_remove)
				{
					DCT_node*temp_node = reinterpret_cast<DCT_node*>(children[i]);
					temp_node->try_clean();
					delete temp_node;
					children.erase(std::remove(children.begin(), children.end(), children[i]), children.end());
				}
				else
				{
					i++;
				}
			}
			else
			{//for DCT component.
				if (children[i]->pending_remove)
				{
					delete children[i];
					children.erase(std::remove(children.begin(), children.end(), children[i]), children.end());
				}
				else
				{
					i++;
				}
			}
		}

		//to do:: after erase, sometimes the memory of the std::vector should be released.

	}

public:
	DCT_node* parent;
	DCT_memory_managing_backend* backend;
	std::vector<DCT_object*> children;

	//to do: possibility for nodes to sort the children and search with binary searching.
};




//component cannot create objects. Call the function from some DCT_node.
class DCT_component :public DCT_object
{
	friend DCT_memory_managing_backend;
public:
	DCT_component() {};
	virtual ~DCT_component() {};
	virtual const char* type() override { return "DCT_component"; };

	/*only node(and if rootnode is developped in some time) has create()
	Can only create components and nodes.
	*/

	bool set_parent(DCT_node* in_parent)
	{
		if (pending_remove)return false;

		if (parent)
		{
			std::vector<DCT_object*> temp = parent->children;

			temp.erase(std::remove(temp.begin(), temp.end(), reinterpret_cast<DCT_object*>(this)), temp.end());//only for sequencial containers, vector deque and string.




			auto iter = parent->children.begin();
		}
			
		parent = in_parent;
		parent->children.push_back(this);
		return true;
		
	}

	//remove() marks the object as pending deleting. It doesnot release memory.
	//To really release memory, call try_clean() from parent node or wait the DCT_memory_managing_tool_backend instance to try_clean() try cleanning all the tree.
	bool remove() override
	{
		pending_remove = true;
		return true;
	}
	virtual bool canContainChild()const override { return false; };

	inline virtual void receive_tick(float delta_time) override
	{//component doesn't have any children. It only needs to tick it self.
		tick(delta_time);
	};

	//virtual void tick(float delta_time)override {};//I shouldn't override this.


public:
	DCT_node* parent;
};




//remember to call tick_object() and clean() frequently.
class DCT_memory_managing_backend
{
public:
	DCT_memory_managing_backend() {};
	~DCT_memory_managing_backend() {};
	virtual const char* type() { return "DCT_memory_managing_backend"; };



	void tick_object(float delta_time)
	{
		for (auto iter = root_nodes.begin(); iter != root_nodes.end(); iter++)
		{//1 root node each time.
			float delat_time = time - time_when_last_tick;
			time_when_last_tick = time;
			(*iter)->receive_tick(delat_time);
		}
	}

	void try_clean_object() 
	{
		for(size_t i = 0;i<root_nodes.size();/*manually control*/)
		
		//for (auto iter = root_nodes.begin(); iter != root_nodes.end(); iter++)
		{//1 root node each time.
			root_nodes[i]->try_clean();
			if (root_nodes[i]->pending_remove)
			{
				auto temp = root_nodes[i];
				delete root_nodes[i];
				root_nodes.erase(std::remove(root_nodes.begin(), root_nodes.end(), temp), root_nodes.end());
			}
			else
			{
				++i;
			}
		}
	};

	void add_root_node(DCT_node* in)
	{
		root_nodes.push_back(in);
		in->__set_parent_invoked_by_DCT_memory_managing_backend(this);
		return;
	}

public:
	std::vector<DCT_node*> root_nodes;
	float time;
	float time_when_last_tick;
};
