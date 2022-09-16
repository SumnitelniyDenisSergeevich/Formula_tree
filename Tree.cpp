#include "Tree.h"

#include <array>
#include <exception>
#include <cmath>
#include <stdexcept>

using namespace std::literals;

size_t CheckSymbolNumber(std::string& fml) {
	size_t count = std::count(fml.begin(), fml.end(), '+');
	count = count + std::count(fml.begin(), fml.end(), '-');
	count = count + std::count(fml.begin(), fml.end(), '*');
	count = count + std::count(fml.begin(), fml.end(), '/');
	count = count + std::count(fml.begin(), fml.end(), '^');
	return count;
}

Node* Tree::ChooseNode(std::string& value, std::map<std::string, Node*>& key_node) {// Добавить выбор из map
	Node* node = nullptr;
	if (value == "x"s) {
		node = new ParametreNode();
	}
	else if (key_node.find(value) != key_node.end()) {
		node = key_node.at(value);
		key_node.erase(value);
	}
	else {
		node = new ConstantNode(std::stod(value));
	}
	return node;
}

Node* Tree::CreateNode(std::string& fml, std::map<std::string, Node*>& key_node) {
	const static std::array<char, 5> symbols{ '+', '-', '*', '/','^' };
	char temp = ' ';
	size_t iter = 0;
	for (const char c : symbols)
		if (iter = fml.find(c); iter != fml.npos) {
			temp = c;
			break;
		}

	std::string left_value, right_value;
	left_value.assign(fml, 0, iter);
	right_value.assign(fml, iter + 1, fml.length() - 1);
	Node* l_val = nullptr;
	if (*fml.begin() == '-') {
		left_value = "0"s;
		l_val = ChooseNode(left_value, key_node);
	}
	else {
		l_val = ChooseNode(left_value, key_node);
	}
	Node* r_val = ChooseNode(right_value, key_node);

	Node* result = new Node(temp);

	result->AddLeftNode(l_val);
	result->AddRightNode(r_val);

	return result;
}

size_t FindSymbolRightToLeft(std::string left_fml) {
	size_t left_iter = left_fml.npos;
	bool find_flag = false;
	for (auto iter = left_fml.rbegin(); iter != left_fml.rend(); ++iter) {
		switch (*iter)
		{
		case '(':
			left_iter = left_fml.rfind('(');
			find_flag = true;
			break;
		case '*':
			left_iter = left_fml.rfind('*');
			find_flag = true;
			break;
		case '/':
			left_iter = left_fml.rfind('/');
			find_flag = true;
			break;
		case '+':
			left_iter = left_fml.rfind('+');
			find_flag = true;
			break;
		case '-':
			left_iter = left_fml.rfind('-');
			find_flag = true;
			break;
		}
		if (find_flag) {
			break;
		}
	}
	return left_iter;
}

size_t FindSymbolLeftToRight(std::string right_fml) {
	size_t right_iter = right_fml.npos;
	bool find_flag = false;
	for (auto iter = right_fml.begin(); iter != right_fml.end(); ++iter) {
		switch (*iter)
		{
		case ')':
			right_iter = right_fml.find(')');
			find_flag = true;
			break;
		case '*':
			right_iter = right_fml.find('*');
			find_flag = true;
			break;
		case '/':
			right_iter = right_fml.find('/');
			find_flag = true;
			break;
		case '+':
			right_iter = right_fml.find('+');
			find_flag = true;
			break;
		case '-':
			right_iter = right_fml.find('-');
			find_flag = true;
			break;
		}
		if (find_flag) {
			break;
		}
	}
	return right_iter;
}

std::string Tree::Code() {
	std::string result = code_;
	if (auto iter = code_.rbegin(); *iter != 'Z') {
		*iter = *iter + 1;
	}
	else {
		*iter = 'A';
		code_ = "A"s + code_;
	}
	return result;
}

void Tree::FmlPow(std::string& fml, const size_t iter, std::map<std::string, Node*>& key_node) {
	size_t left_iter = fml.npos, right_iter = fml.npos;
	std::string left_fml, right_fml, mini_tree;
	left_fml.assign(fml, 0, iter);
	right_fml.assign(fml, iter + 1, fml.length() - 1 - iter);
	left_iter = FindSymbolRightToLeft(left_fml);
	if (left_iter == fml.npos) {
		left_iter = 0;
	}
	right_iter = FindSymbolLeftToRight(right_fml);
	if (right_iter == fml.npos) {
		right_iter = right_fml.length();
	}
	std::string code = Code();
	if (left_iter == 0) {
		mini_tree.assign(fml, 0, iter + 1 + right_iter);
		fml.replace(0, mini_tree.length(), code);
	}
	else {
		mini_tree.assign(fml, left_iter + 1, iter - left_iter + right_iter);
		fml.replace(left_iter + 1, mini_tree.length(), code);
	}
	key_node[code] = CreateTree(mini_tree);
}

void Tree::FmlBracket(std::string& fml, const size_t iter, std::map<std::string, Node*>& key_node) {
	std::string left_fml, mini_tree;
	left_fml.assign(fml, 0, iter);
	mini_tree.assign(left_fml, left_fml.rfind('(') + 1, left_fml.length() - 1);
	std::string code = Code();
	fml.replace(left_fml.rfind('('), mini_tree.length() + 2, code); // +2 для убирания скобок
	key_node[code] = CreateTree(mini_tree);
}

void Tree::FmlMultiplicationDivision(std::string& fml, std::map<std::string, Node*>& key_node) {
	size_t left_iter = fml.npos, right_iter = fml.npos, iter = fml.find('*') != fml.npos ? fml.find('*') : fml.find('/');
	std::string left_fml, right_fml, mini_tree;
	left_fml.assign(fml, 0, iter);
	right_fml.assign(fml, iter + 1, fml.length() - 1 - iter);
	left_iter = FindSymbolRightToLeft(left_fml);
	if (left_iter == fml.npos) {
		left_iter = 0;
	}
	right_iter = FindSymbolLeftToRight(right_fml);
	if (right_iter == fml.npos) {
		right_iter = right_fml.length();
	}
	std::string code = Code();
	if (left_iter == 0 && *fml.begin() != '-') {
		mini_tree.assign(fml, 0, iter + 1 + right_iter);
		fml.replace(0, mini_tree.length(), code);
	}
	else {
		mini_tree.assign(fml, left_iter + 1, iter - left_iter + right_iter);
		fml.replace(left_iter + 1, mini_tree.length(), code);
	}
	key_node[code] = CreateTree(mini_tree);
}

void Tree::FmlPlusMinus(std::string& fml, std::map<std::string, Node*>& key_node) {
	size_t right_iter = fml.npos, iter = fml.find('+') != fml.npos ? fml.find('+') : fml.find('-');
	std::string right_fml, mini_tree;
	right_fml.assign(fml, iter + 1, fml.length() - iter - 1);
	for (auto iter = right_fml.begin(); iter != right_fml.end(); ++iter) {
		if (*iter == '+') {
			right_iter = right_fml.find('+');
			break;
		}
		else if (*iter == '-') {
			right_iter = right_fml.find('-');
			break;
		}
	}
	if (right_iter == fml.npos) {
		right_iter = right_fml.length();
	}
	mini_tree.assign(fml, 0, iter + right_iter + 1);
	std::string code = Code();
	fml.replace(0, mini_tree.length(), code);
	key_node[code] = CreateTree(mini_tree);
}

void Tree::FmlUnaryPlusMinus(std::string& fml, std::map<std::string, Node*>& key_node) {
	size_t right_iter = fml.npos;
	std::string right_fml, mini_tree;
	right_fml.assign(fml, 1, fml.length() - 1);
	for (auto iter = right_fml.begin() + 1; iter != right_fml.end(); ++iter) {
		if (*iter == '+') {
			right_iter = right_fml.find('+');
			break;
		}
		else if (*iter == '-') {
			right_iter = right_fml.find('-');
			break;
		}
	}
	if (right_iter == fml.npos) {
		right_iter = right_fml.length();
	}
	mini_tree.assign(fml, 0, right_iter + 1);
	std::string code = Code();
	fml.replace(0, mini_tree.length(), code);
	key_node[code] = CreateTree(mini_tree);
}

Node* Tree::CreateTree(std::string& fml) {
	static std::map<std::string, Node*> key_node;
	size_t count = CheckSymbolNumber(fml);
	if (count == 1) {
		return CreateNode(fml, key_node);
	}
	else if (count == 0) {
		return ChooseNode(fml, key_node);
	}

	if (size_t iter = fml.find('^'); iter != fml.npos) {
		FmlPow(fml, iter, key_node);
		return CreateTree(fml);
	}
	else if (size_t iter = fml.find(')'); iter != fml.npos) {// если нашел закрывающую скобку
		FmlBracket(fml, iter, key_node);
		return CreateTree(fml);
	}
	else if (fml.find('*') != fml.npos || fml.find('/') != fml.npos) {
		FmlMultiplicationDivision(fml, key_node);
		return CreateTree(fml);
	}
	else if (fml.find('+') != fml.npos || fml.find('-') != fml.npos) {
		// для унарного минуса, проверить стоит ли он первым
		if (*fml.begin() == '-') {
			FmlUnaryPlusMinus(fml, key_node);
		}
		else {
			FmlPlusMinus(fml, key_node);
		}
		return CreateTree(fml);
	}
}

double Node::Calc(const double parametre) const {
	if (!symbol_.has_value()) {
		throw std::invalid_argument("Символ не обнаружен!"s);
	}
	double l = left_node_->Calc(parametre), r = right_node_->Calc(parametre);
	switch (symbol_.value())
	{
	case '+':
		return l + r;
		break;
	case '-':
		return l - r;
		break;
	case '*':
		return l * r;
		break;
	case '/':
		return l / r;
		break;
	case '^':
		return std::pow(l, r);
		break;
	default:
		throw std::invalid_argument("Символ не стандартный!"s);
		return 0;
	}
}

double ConstantNode::Calc(const double parametre) const {
	return value_;
}

double ParametreNode::Calc(const double parametre) const {
	return parametre;
}

Node::~Node() {
	if (left_node_ != nullptr) {
		delete left_node_;
	}
	if (right_node_ != nullptr) {
		delete right_node_;
	}
}

Node::Node(const char symbol) : symbol_(symbol) {
}

void Node::AddLeftNode(Node* const l_node) {
	left_node_ = l_node;
}

void Node::AddRightNode(Node* const r_node) {
	right_node_ = r_node;
}

double Tree::Calc(const double parametre) {
	return root_->Calc(parametre);
}

ConstantNode::ConstantNode(const double value) : value_(value) {

}

Tree::Tree(std::string& fml) : code_("A") { // начинаем создавать Дерево // начало x+1
	root_ = CreateTree(fml);
}

Tree::~Tree() {
	if (root_ != nullptr) {
		delete root_;
	}
}