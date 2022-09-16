#pragma once

#include <optional>
#include <string>
#include <map>

class Node
{
public:
	Node() = default;
	~Node();
	Node(const char symbol);
	virtual double Calc(const double parametre) const;
	void AddLeftNode(Node* const);
	void AddRightNode(Node* const);
private:
	std::optional<char> symbol_ = std::nullopt;
	Node* left_node_ = nullptr;
	Node* right_node_ = nullptr;
};

class ConstantNode : public Node {
public:
	ConstantNode(const double value);
	double Calc(const double parametre) const override;
private:
	double value_;
};

class ParametreNode : public Node {
public:
	double Calc(const double parametre) const override;
};

class Tree {
public:
	Tree(std::string&);
	~Tree();
	double Calc(const double parametre);
private:
	std::string code_;
	Node* root_;

	Node* CreateTree(std::string& fml);
	std::string Code();
	void FmlPow(std::string& fml, const size_t iter, std::map<std::string, Node*>& key_node);
	void FmlBracket(std::string& fml, const size_t iter, std::map<std::string, Node*>& key_node);
	void FmlMultiplicationDivision(std::string& fml, std::map<std::string, Node*>& key_node);
	void FmlPlusMinus(std::string& fml, std::map<std::string, Node*>& key_node);
	void FmlUnaryPlusMinus(std::string& fml, std::map<std::string, Node*>& key_node);
	Node* ChooseNode(std::string& value, std::map<std::string, Node*>& key_node);
	Node* CreateNode(std::string& fml, std::map<std::string, Node*>& key_node);
};

