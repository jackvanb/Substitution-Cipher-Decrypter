//
//  Header.h
//  CS32-Proj4
//
//  Created by Jack Van Boening on 3/10/18.
//  Copyright © 2018 Jack Van Boening. All rights reserved.
//

#ifndef MyHash_h
#define MyHash_h
// MyHash.h

const int DEFAULT_HASH_SIZE = 100;

template<typename KeyType, typename ValueType>
class MyHash
{
public:
    MyHash(double maxLoadFactor = 0.5) : m_bucketSize(DEFAULT_HASH_SIZE), m_numItems(0)
    {
        m_bucket = new Node*[m_bucketSize];
        for (int i = 0; i < m_bucketSize; i++)
            m_bucket[i] = nullptr;
        
        if (maxLoadFactor <= 0)
            m_maxLoadFactor = 0.5;
        else if (maxLoadFactor > 2.0)
            m_maxLoadFactor = 2.0;
        else
            m_maxLoadFactor = maxLoadFactor;
    }
    
    ~MyHash()
    {
        for (int i = 0; i < m_bucketSize; i++)
        {
            Node* entry = m_bucket[i];
            while (entry != nullptr)
            {
                Node * deleteMe = entry;
                entry = entry->next;
                delete deleteMe;
                
            }
            m_bucket[i] = nullptr;
        }
        delete [] m_bucket;
    }
    
    void reset()
    {
        for (int i = 0; i < m_bucketSize; i++)
        {
            Node* entry = m_bucket[i];
            while (entry != nullptr)
            {
                Node * deleteMe = entry;
                entry = entry->next;
                delete deleteMe;
                
            }
            m_bucket[i] = nullptr;
        }
        delete [] m_bucket;
        
        
        m_bucket = new Node*[DEFAULT_HASH_SIZE];
        m_numItems = 0;
        m_bucketSize = DEFAULT_HASH_SIZE;
        for (int i = 0; i < DEFAULT_HASH_SIZE; i++)
            m_bucket[i] = nullptr;
    }
    
    void associate(const KeyType& key, const ValueType& value)
    {
        put(key, value);
        
        // Check to see if it is past max load factor
        if (getLoadFactor() > m_maxLoadFactor)
        {
            // Create New Hash Table With Double The Size
            resize();
        }
    }
    
    // for a map that can't be modified, return a pointer to const ValueType
    const ValueType* find(const KeyType& key) const
    {
        unsigned int hash_val = getBucketNumber(key);
        
        if (m_bucket[hash_val] == nullptr)
            return nullptr;
        else
        {
            Node* entry = m_bucket[hash_val];
            while (entry != nullptr)
            {
                if (entry->m_key == key)
                    return &entry->m_value;
                else
                    entry = entry->next;
            }
        }
        return nullptr;
    }
    
    // for a modifiable map, return a pointer to modifiable ValueType
    ValueType* find(const KeyType& key)
    {
        return const_cast<ValueType*>(const_cast<const MyHash*>(this)->find(key));
    }
    
    //  SIMPLE FUNCTIONS
    int getNumItems() const { return m_numItems; }
    double getLoadFactor() const { return (m_numItems * 1.0) / m_bucketSize; }
    
    // C++11 syntax for preventing copying and assignment
    MyHash(const MyHash&) = delete;
    MyHash& operator=(const MyHash&) = delete;
    
private:
    struct Node
    {
        KeyType   m_key;
        ValueType m_value;
        Node * next;
    };
    Node ** m_bucket;    // Dynmaic Array of Linked Lists
    
    unsigned int getBucketNumber(const KeyType& key) const
    {
        unsigned int hash(const KeyType& k);  // prototype
        unsigned int h = hash(key);
        return h % m_bucketSize;
    }
    
    void resize()
    {
        int oldTableSize = m_bucketSize;
        m_bucketSize *= 2;
        
        Node ** oldTable = m_bucket;
        m_bucket = new Node*[m_bucketSize];
        for (int i = 0; i < m_bucketSize; i++)
            m_bucket[i] = nullptr;
        
        m_numItems = 0;
        
        for (int hash = 0; hash < oldTableSize; hash++)
        {
            if (oldTable[hash] != nullptr)
            {
                Node * oldEntry;
                Node * entry = oldTable[hash];
                while (entry != nullptr)
                {
                    put(entry->m_key, entry->m_value);
                    oldEntry = entry;
                    entry = entry->next;
                    delete oldEntry;
                }
            }
        }
        delete [] oldTable;
    }
    
    void put(const KeyType& key, const ValueType& value)
    {
        if (find(key) != nullptr)
        {
            // Already in Hash Table, Update Value
            *(find(key)) = value;
            return;
        }
        
        int hash_val = getBucketNumber(key);
        Node* entry = m_bucket[hash_val];
        
        m_numItems++;
        if (entry != nullptr)
        {
            while (entry->next != nullptr)
            {
                entry = entry->next;
            }
            entry->next = new Node;
            
            entry->next->m_key = key;
            entry->next->m_value = value;
            entry->next->next = nullptr;
        }
        else
        {
            m_bucket[hash_val] = new Node;
            m_bucket[hash_val]->m_key = key;
            m_bucket[hash_val]->m_value = value;
            m_bucket[hash_val]->next = nullptr;
        }
        
    }
    

    double m_maxLoadFactor;
    int m_bucketSize;
    int m_numItems;
    
};

#endif /* MyHash_h */
