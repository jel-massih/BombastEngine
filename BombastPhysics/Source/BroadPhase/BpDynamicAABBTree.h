#pragma once

#include <cassert>
#include "../Geometry/BpGeometry.h"

namespace bPhysics
{

	class BpDynamicAABBTree
	{
	public:
		BpDynamicAABBTree();
		~BpDynamicAABBTree();

		int Insert(const BpAABB& aabb, void* userData);
		void Remove(int id);
		bool Update(int id, const BpAABB& aabb);

		void* GetUserData(int id) const;
		const BpAABB& GetFatAABB(int id) const;
		
		template <typename T>
		inline void Query(T* cb, const BpAABB& aabb) const;
		template <typename T>
		inline void Query(T* cb, BpRaycastData& raycast) const;

		void Validate() const;

	private:
		struct Node
		{
			bool IsLeaf() const
			{
				return right == -1;
			}

			BpAABB aabb;

			union
			{
				int parent;
				int next;
			};

			struct
			{
				int left;
				int right;
			};

			void* userData;
			int height;
		};

		inline int AllocateNode();
		inline void DeallocateNode(int index);
		int BalanceTree(int index);

		void InsertLeaf(int index);
		void RemoveLeaf(int index);
		void ValidateStructure(int index) const;
	
		//Correct AABB heirarchy heights and aabbs from index up the tree
		void SyncHeirarchy(int index);

		inline void AddToFreeList(int index);

		int m_root;
		Node* m_nodes;
		int m_count;
		int m_capacity;
		int m_freeList;
	};

	inline void BpDynamicAABBTree::AddToFreeList(int index)
	{
		for (int i = index; i < m_capacity - 1; i++)
		{
			m_nodes[i].next = i + 1;
			m_nodes[i].height = -1;
		}

		m_nodes[m_capacity - 1].next = -1;
		m_nodes[m_capacity - 1].height = -1;
		m_freeList = index;
	}

	inline void BpDynamicAABBTree::DeallocateNode(int index)
	{
		assert(index >= 0 && index < m_capacity);

		m_nodes[index].next = m_freeList;
		m_nodes[index].height = -1;
		m_freeList = index;

		m_count--;
	}

	template <typename T>
	inline void BpDynamicAABBTree::Query(T* cb, const BpAABB& aabb) const
	{
		const int stackCapacity = 256;
		int stack[stackCapacity];
		int sp = 1;

		*stack = m_root;

		while (sp)
		{
			assert(sp < stackCapacity);
			int id = stack[--sp];

			const Node* n = m_nodes + id;

			if (AABBtoAABB(aabb, n->aabb))
			{
				if (n->IsLeaf())
				{
					if (!cb->TreeCallback(id))
						return;
				}
				else
				{
					stack[sp++] = n->left;
					stack[sp++] = n->right;
				}
			}
		}
	}

	template <typename T>
	inline void BpDynamicAABBTree::Query(T* cb, const BpRaycastData& rayCast) const
	{
		const float epsilon = 0.000001f;
		const int stackCapacity = 256;
		int stack[stackCapacity];
		int sp = 1;

		*stack = m_root;

		while (sp)
		{
			assert(sp < stackCapacity);
			int id = stack[--sp];

			const Node* n = m_nodes + id;

			if (AABBtoAABB(aabb, n->aabb))
			{
				if (n->IsLeaf())
				{
					if (!cb->TreeCallback(id))
						return;
				}
				else
				{
					stack[sp++] = n->left;
					stack[sp++] = n->right;
				}
			}
		}
	}
}