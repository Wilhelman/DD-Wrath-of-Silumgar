#ifndef __p2List_H__
#define __p2List_H__

#include "ctDefs.h"

/**
* Contains items from double linked list
*/
template<class tdata>
struct ctList_item
{
	tdata                 data;
	ctList_item<tdata>*   next;
	ctList_item<tdata>*   prev;

	inline ctList_item(const tdata& _data)
	{
		data = _data;
		next = prev = NULL;
	}

	~ctList_item()
	{}
};

/**
* Manages a double linked list
*/
template<class tdata>
class ctList
{

public:

	ctList_item<tdata>*   start;
	ctList_item<tdata>*   end;

private:

	unsigned int  size;

public:

	/**
	* Constructor
	*/
	inline ctList()
	{
		start = end = NULL;
		size = 0;
	}

	/**
	* Destructor
	*/
	~ctList()
	{
		clear();
	}

	/**
	* Get Size
	*/
	unsigned int count() const
	{
		return size;
	}

	/**
	* Add new item
	*/
	ctList_item<tdata>* add(const tdata& item)
	{
		ctList_item<tdata>*   p_data_item;
		p_data_item = new ctList_item < tdata >(item);

		if (start == NULL)
		{
			start = end = p_data_item;
		}
		else
		{
			p_data_item->prev = end;
			end->next = p_data_item;
			end = p_data_item;
		}

		++size;
		return(p_data_item);
	}

	/**
	* Deletes an item from the list
	*/
	bool del(ctList_item<tdata>* item)
	{
		if (item == NULL)
		{
			return (false);
		}

		// Now reconstruct the list
		if (item->prev != NULL)
		{
			item->prev->next = item->next;

			if (item->next != NULL)
			{
				item->next->prev = item->prev;
			}
			else
			{
				end = item->prev;
			}
		}
		else
		{
			if (item->next)
			{
				item->next->prev = NULL;
				start = item->next;
			}
			else
			{
				start = end = NULL;
			}
		}

		RELEASE(item);
		--size;
		return(true);
	}

	/**
	* Destroy and free all mem
	*/
	void clear()
	{
		ctList_item<tdata>*   p_data;
		ctList_item<tdata>*   p_next;
		p_data = start;

		while (p_data != NULL)
		{
			p_next = p_data->next;
			RELEASE(p_data);
			p_data = p_next;
		}

		start = end = NULL;
		size = 0;
	}

	/**
	* read / write operator access directly to a position in the list
	*/
	tdata& operator  [](const unsigned int index)
	{
		long                  pos;
		ctList_item<tdata>*   p_item;
		pos = 0;
		p_item = start;

		while (p_item != NULL)
		{
			if (pos == index)
			{
				break;
			}

			++pos;
			p_item = p_item->next;
		}

		return(p_item->data);
	}

	/**
	* const read operator access directly to a position in the list
	*/
	const tdata& operator  [](const unsigned int index) const
	{
		long                  pos;
		ctList_item<tdata>*   p_item;
		pos = 0;
		p_item = start;

		while (p_item != NULL)
		{
			if (pos == index)
			{
				break;
			}

			++pos;
			p_item = p_item->next;
		}

		ASSERT(p_item);

		return(p_item->data);
	}

	/**
	* const read operator access directly to a position in the list
	*/
	const ctList<tdata>& operator +=(const ctList<tdata>& other_list)
	{
		ctList_item<tdata>*   p_item = other_list.start;

		while (p_item != NULL)
		{
			add(p_item->data);
			p_item = p_item->next;
		}

		return(*this);
	}

	/**
	* const access to a node in a position in the list
	*/
	const ctList_item<tdata>* At(unsigned int index) const
	{
		long                  pos = 0;
		ctList_item<tdata>*   p_item = start;

		while (p_item != NULL)
		{
			if (pos++ == index)
				break;

			p_item = p_item->next;
		}

		return p_item;
	}

	/**
	* access to a node in a position in the list
	*/
	ctList_item<tdata>* At(unsigned int index)
	{
		long                  pos = 0;
		ctList_item<tdata>*   p_item = start;

		while (p_item != NULL)
		{
			if (pos++ == index)
				break;

			p_item = p_item->next;
		}

		return p_item;
	}

	// Sort
	int BubbleSort()
	{
		int ret = 0;
		bool swapped = true;

		while (swapped)
		{
			swapped = false;
			ctList_item<tdata>* tmp = start;

			while (tmp != NULL && tmp->next != NULL)
			{
				++ret;
				if (tmp->data > tmp->next->data)
				{
					SWAP(tmp->data, tmp->next->data);
					swapped = true;
				}

				tmp = tmp->next;
			}
		}

		return ret;
	}

	/**
	* returns the first apperance of data as index (-1 if not found)
	*/
	int find(const tdata& data)
	{
		ctList_item<tdata>* tmp = start;
		int index = 0;

		while (tmp != NULL)
		{
			if (tmp->data == data)
				return(index);

			++index;
			tmp = tmp->next;
		}
		return (-1);
	}

	void InsertAfter(uint position, const ctList<tdata>& list)
	{
		ctList_item<tdata>* p_my_list = At(position);
		ctList_item<tdata>* p_other_list = list.start;

		while (p_other_list != NULL)
		{
			ctList_item<tdata>* p_new_item = new p2List_item<tdata>(p_other_list->data);

			p_new_item->next = (p_my_list) ? p_my_list->next : NULL;

			if (p_new_item->next != NULL)
				p_new_item->next->prev = p_new_item;
			else
				end = p_new_item;

			p_new_item->prev = p_my_list;

			if (p_new_item->prev != NULL)
				p_new_item->prev->next = p_new_item;
			else
				start = p_new_item;

			p_my_list = p_new_item;
			p_other_list = p_other_list->next;
		}
	}
};
#endif /*__p2List_H__*/