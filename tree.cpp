//
// Created by JKearnsl on 01.05.2022.
//

#include "tree.h"
#include <iostream>
#include <fstream>

node *Tree::getRoot()
{
    return this->root;
}

bool Tree::save(const std::string& filepath)
/*
 *  Метод сохраняет
 *  Дерево в указанный filepath
 */
{
    std::vector<int> values = getPreorderValues();

    std::ofstream file(filepath);
    if (file.is_open())
    {
        for (int i = 0; i < values.size(); i++)
        {
            file << values[i] << std::endl;
        }
        file.close();
        return true;
    } else
        return false;
}

int Tree::load(const std::string& filepath)
/*
 *  Метод загружает дерево из файла
 *  :filepath:
 *
 *  Вернет:
 *
 *  0 - если дерево успешно загружено
 *  1 - если файл не открылся
 *  2 - если файл пуст
 *  3 - Ошибка содержания файла
 *
 */
{
    if (!this->is_file_not_exist(filepath))
    {
        if (!this->is_empty_file(filepath))
        {
            this->root = nullptr;
            std::ifstream file(filepath);
            std::string temp_str;
            while (getline(file, temp_str))
            {
                try
                {
                    (void) std::stoi(temp_str);
                }
                catch (const std::logic_error &e)
                {
                    file.close();
                    return 3;
                }
                this->insert(std::stoi(temp_str));
            }
            file.close();
            return 0;
        } else
            return 2;
    } else
        return 1;
}

std::vector<int> Tree::getInorderValues()
/*
 *  Метод возвращает отсортированный список
 *  элементов дерева (обход узлов в отсортированном порядке)
 *
 */
{
    std::vector<int> result;
    this->_inorder(this->root, result);
    return result;
}

std::vector<int> Tree::getPreorderValues()
/*
 *  Метод возвращает первоначальный список
 *  элементов дерева (так, как было иниц)
 *
 */
{
    std::vector<int> result;
    this->_preorder(this->root, result);
    return result;
}

std::vector<int> Tree::getPostorderValues()
/*
 *  Метод возвращает отсортированный список элементов дерева
 *  (обход узлов в порядке: левое поддерево, правое поддерево, вершина)
 */
{
    std::vector<int> result;
    this->_postorder(this->root, result);
    return result;
}



bool Tree::insert(int value)
/*
 *  Метод вставляет элемент value
 *  В дерево
 *
 */
{
    try
    {
        this->root = _add_node(this->root, value);
    }
    catch( const std::invalid_argument& error )
    {
        std::cout << "Ошибка добавления элемента '" << value << "' в дерево: " << error.what() << std::endl;
        return false;
    }
    return true;
}

bool Tree::search(int value)
/*
 *  Метод ищет элемент :value:
 *  В дереве, если элемент найден,
 *  то метод вернет true, в противном
 *  случае - false
 */
{
    return this->_search(this->root,  value);
}

bool Tree::remove(int value)
/*
 *  Метод удаляет элемент :value:
 *  из дерева
 */
{
    if (this->search(value))
    {
        this->_deleteNode(this->root, value);
        return true;
    } else
        return false;
}


void Tree::_inorder(node *_node, std::vector<int> &result)
/*
 * обход узлов в отсортированном порядке,
 *
 *
 */
{
    if (_node != nullptr)
    {
        // Обходим лево
        this->_inorder(_node->left, result);

        // Обходим корень
        result.push_back(_node->key);

        // Обходим право
        this->_inorder(_node->right, result);

    }
}

void Tree::_preorder(node *_node, std::vector<int> &result)
/*
 * обход узлов в порядке: вершина, левое поддерево, правое поддерево
 *
 */
{
    if (_node != nullptr)
    {
        result.push_back(_node->key);

        // Обходим лево
        this->_preorder(_node->left, result);

        // Обходим право
        this->_preorder(_node->right, result);
    }
}

void Tree::_postorder(node *_node, std::vector<int> &result)
/*
 * обход узлов в порядке: вершина, левое поддерево, правое поддерево
 *
 */
{
    if (_node != nullptr)
    {
        // Обходим лево
        this->_postorder(_node->left, result);

        // Обходим право
        this->_postorder(_node->right, result);
        result.push_back(_node->key);
    }
}


bool Tree::_search(node* _node, int value)
{
    if (_node == nullptr)
        return false;

    if (value == _node->key)
        return true;

    if (value < _node->key)
        return this->_search(_node->left, value);
    if (value > _node->key)
        return this->_search(_node->right, value);
    return false;
}

node *Tree::_deleteNode(node *&_node, int key)
{
    // Возвращаем, если дерево пустое
    if (_node == nullptr) return _node;

    // Ищем узел, который нужно удалить
    if (key < _node->key)
        _node->left = this->_deleteNode(_node->left, key);
    else if (key > _node->key)
        _node->right = this->_deleteNode(_node->right, key);
    else if ((_node->left != nullptr) && (_node->right != nullptr))
    {
        _node->key = this->_minValueNode(_node->right)->key;
        _node->right = this->_deleteNode(_node->right, _node->key);
    } else
    {
        if (_node->left != nullptr)
            _node = _node->left;
        else if (_node->right != nullptr)
            _node = _node->right;
        else
            _node = nullptr;
    }
    return _node;
}

node *Tree::_add_node(node *_node, int key)
{
    if (_node == nullptr)
    {
        node *new_node = new node;
        new_node->key = key;
        return new_node;
    }

    if (_node->key == key)
        throw std::invalid_argument("Дерево не может содержать одинаковые значения");

    // Проходим в нужное место и вставляем узел
    if (key < _node->key)
        _node->left = this->_add_node(_node->left, key);
    else
        _node->right = this->_add_node(_node->right, key);

    return _node;
}

void Tree::clear()
// Метод очищает дерево; todo: оптимизировать
{
    if (this->root != nullptr)
    {
        std::vector<int> values = this->getPreorderValues();
        for (int i = 0; i < values.size(); i++)
        {
            this->remove(values[i]);
        }
    }
    this->root = nullptr;
}



node *Tree::_minValueNode(node *_node)
{
    struct node *current = _node;

    // Находим крайний левый лист — он и будет inorder-преемником
    while (current && current->left != nullptr)
        current = current->left;

    return current;
}

bool Tree::is_empty_file(const std::string& file_path)
{
    std::ifstream file(file_path);
    file.seekg(0, std::ios::end);
    long long file_size = file.tellg();
    file.close();

    if (file_size)
        return false;
    else
        return true;
}

bool Tree::is_file_not_exist(const std::string& file_path)
{
    std::ifstream file(file_path);
    bool is_exist = file.is_open();
    file.close();

    if (is_exist)
    {
        return false;
    }
    else
        return true;
}
