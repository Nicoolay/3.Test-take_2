#define CATCH_CONFIG_MAIN
#include <iostream>
#include <catch2/catch_test_macros.hpp>
//#include "catch2/catch_test_macros.hpp"
struct ListNode
{
public:
    ListNode(int value, ListNode* prev = nullptr, ListNode* next = nullptr)
        : value(value), prev(prev), next(next)
    {
        if (prev != nullptr) prev->next = this;
        if (next != nullptr) next->prev = this;
    }

public:
    int value;
    ListNode* prev;
    ListNode* next;
};


class List
{
public:
    List()
        : m_head(new ListNode(static_cast<int>(0))), m_size(0),
        m_tail(new ListNode(0, m_head))
    {
    }

    virtual ~List()
    {
        Clear();
        delete m_head;
        delete m_tail;
    }

    bool Empty() { return m_size == 0; }

    unsigned long Size() { return m_size; }

    void PushFront(int value)
    {
        new ListNode(value, m_head, m_head->next);
        ++m_size;
    }

    void PushBack(int value)
    {
        new ListNode(value, m_tail->prev, m_tail);
        ++m_size;
    }

    int PopFront()
    {
        if (Empty()) throw std::runtime_error("list is empty");
        auto node = extractPrev(m_head->next->next);
        int ret = node->value;
        delete node;
        return ret;
    }

    int PopBack()
    {
        if (Empty()) throw std::runtime_error("list is empty");
        auto node = extractPrev(m_tail);
        int ret = node->value;
        delete node;
        return ret;
    }

    void Clear()
    {
        auto current = m_head->next;
        while (current != m_tail)
        {
            current = current->next;
            delete extractPrev(current);
        }
    }

private:
    ListNode* extractPrev(ListNode* node)
    {
        auto target = node->prev;
        target->prev->next = target->next;
        target->next->prev = target->prev;
        --m_size;
        return target;
    }

private:
    ListNode* m_head;
    ListNode* m_tail;
    unsigned long m_size;
};

TEST_CASE("List::Empty", "[list]") {
    List list;

    SECTION("Свежесозданный список пуст") {
        REQUIRE(list.Empty() == true);
    }

    SECTION("Список не пуст после добавления элемента") {
        list.PushFront(10);
        REQUIRE(list.Empty() == false);
    }

    SECTION("Список снова пуст после удаления всех элементов") {
        list.PushFront(10);
        list.PopFront();
        REQUIRE(list.Empty() == true);
    }
}

TEST_CASE("List::Size", "[list]") {
    List list;

    SECTION("Размер нового списка равен 0") {
        REQUIRE(list.Size() == 0);
    }

    SECTION("Размер увеличивается при добавлении элементов") {
        list.PushBack(10);
        REQUIRE(list.Size() == 1);

        list.PushBack(20);
        REQUIRE(list.Size() == 2);
    }

    SECTION("Размер уменьшается при удалении элементов") {
        list.PushBack(10);
        list.PushBack(20);
        list.PopFront();
        REQUIRE(list.Size() == 1);

        list.PopBack();
        REQUIRE(list.Size() == 0);
    }
}

TEST_CASE("List::Clear", "[list]") {
    List list;

    SECTION("Очистка пустого списка не вызывает ошибок") {
        REQUIRE_NOTHROW(list.Clear());
        REQUIRE(list.Size() == 0);
        REQUIRE(list.Empty() == true);
    }

    SECTION("Очистка списка удаляет все элементы") {
        list.PushFront(10);
        list.PushFront(20);
        list.PushFront(30);

        REQUIRE(list.Size() == 3);
        list.Clear();
        REQUIRE(list.Size() == 0);
        REQUIRE(list.Empty() == true);
    }
}