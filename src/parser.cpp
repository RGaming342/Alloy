#include "../include/parser.h"
#include <algorithm>
#include <iostream>




Node* Parser::Start(){
	Node* node = new Node("Prog");
	
	while(peek().h){
		if(peek(2).h&&Type(peek().t)=="Type"&&Type(peek(1).t)=="Ident"&&peek(2).t.value=="("){// func: type name (
			node->children.push_back(Function());
		}
		else if(peek(1).h&&Type(peek().t)=="Type"&&Type(peek(1).t)=="Ident"){// var: type name (;)optional
			node->children.push_back(Variable());
		}
		else if(peek(2).h&&Type(peek(1).t)=="Ident"&&peek(2).t.value==";"){// var: error name ;
			node->children.push_back(Variable());
		}else{
			node->children.push_back(new Node("ERROR",peek().t.value));
			inc();
		}
	}
	
	return node;
}

Node* Parser::Variable(){
	Node* node = new Node("Var");
	if(peek(2).h&&Type(peek().t)=="Type"&&Type(peek(1).t)=="Ident"&&peek(2).t.value==";"){
		Token t = inc();
		node->children.push_back( new Node("Type",t.value,t) );
		t = inc();
		node->children.push_back( new Node("Ident",t.value,t) );
		inc();// skip ;
	}else if(peek(1).h&&Type(peek().t)=="Type"&&Type(peek(1).t)=="Ident"){
		Token t = inc();
		node->children.push_back( new Node("Type",t.value,t) );
		t = inc();
		node->children.push_back( new Node("Ident",t.value,t) );
		Node* tmp = new Node("ERROR","",t);
		tmp->addError("missing");
		tmp->addError(";");
		node->children.push_back( tmp );
		std::cout<<"Var missing semi(;)"<<std::endl;
	}else if(peek(2).h&&Type(peek(1).t)=="Ident"&&peek(2).t.value==";"){
		Token t = inc();
		node->children.push_back( new Node("ERROR",t.value,t) );
		t = inc();
		node->children.push_back( new Node("Ident",t.value,t) );
		inc();// skip ;
		std::cout<<"Var: type is not valid"<<std::endl;
	}
	return node;
}

Node* Parser::Function(){
	return nullptr;
}












Parser::Parser(){}
Parser::~Parser(){}
Node* Parser::parse(std::vector<Token> _tokens){
	tokens = std::move(_tokens);
	return Start();
}
Token Parser::inc(){
	return tokens.at(index++);
}

Parser::PeekRet Parser::peek(int offset) const {
	if (index + offset >= tokens.size()) {
		return {0,{-1,"",0,0}};
	}
	return {1,tokens[index + offset]};
}

std::string Parser::Type(Token tok){
	return TokenTypes[tok.type];
}





/*
void Parser::printASTsub(Node* AST, Node* parent,int lmr) {
	if (AST == nullptr) return;
	
	if (AST->type == NodeType("Null")) return;
	
	if (parent != nullptr) {
		std::cout <<NodeType(parent->type)<<lmr;
		if(parent->data!=""){std::cout<<"("<< parent->data <<")"<<" -> ";}else{std::cout<<" -> ";}
	}
	std::cout << NodeType(AST->type);
	if(AST->data!=""){std::cout<<"("<< AST->data <<")";}
	
	std::cout << std::endl;
	
	printASTsub(AST->left, AST,0);
	printASTsub(AST->middle, AST,1);
	printASTsub(AST->right, AST,2);
}
void Parser::printAST(Node* AST) {
	if (AST != nullptr) {
		std::cout<<".\n";
		printASTsub(AST);
	}
}



Node* Parser::helpFunc(std::string id,Lexer& lr){
	if (helperFuncs.find(id) != helperFuncs.end()) {
		return helperFuncs[id]->run(lr, *this);
	} else {
		std::cerr << "Helper function with id \"" << id << "\" not found.\n";
		return nullptr;
	}
}
void Parser::addHelperFunc(std::string id, HelperFunction* func){
	helperFuncs[id] = func;
}
void Parser::addChildHelperFunc(std::string parentId, int childId, HelperFunction* func){
	if (helperFuncs.find(parentId) != helperFuncs.end()) {
		helperFuncs[parentId]->addChild(childId, func);
	} else {
		std::cerr << "Parent helper function with id \"" << parentId << "\" not found.\n";
	}
}
void Parser::addData(std::string funcId, std::string id, int data){
	if (helperFuncs.find(funcId) != helperFuncs.end()) {
		helperFuncs[funcId]->dataST0[id] = data;
	} else {
		std::cerr << "Helper function with id \"" << funcId << "\" not found.\n";
	}
}
void Parser::addData(std::string funcId, std::string data){
	if (helperFuncs.find(funcId) != helperFuncs.end()) {
		helperFuncs[funcId]->dataST1.push_back(data);
	} else {
		std::cerr << "Helper function with id \"" << funcId << "\" not found.\n";
	}
}



Parser::Parser(std::vector<Token>& toks) : tokens(toks) {}

Node::Node(int type,std::string data) : type(type), data(data), left(nullptr), middle(nullptr), right(nullptr){};
Node::~Node() {
	delete left;
	delete middle;
	delete right;
}

void Parser::addNodeType(std::string type) {
	for (unsigned int i = 0; i < NodeTypes.size(); i++) {
		if (NodeTypes[i] == type) {
			return;
		}
	}
	NodeTypes.push_back(type);
}

int Parser::NodeType(std::string type) {
	for (unsigned int i = 0; i < NodeTypes.size(); i++) {
		if (NodeTypes[i] == type) {
			return i;
		}
	}
	if(Enable){
		Err("invalid Node Type: "+ type);
	}
	return -1;
}
std::string Parser::NodeType(unsigned int type) {
	if(type < NodeTypes.size()){
		return NodeTypes[type];
	}
	if(Enable){
		Err("invalid Token Type: "+std::to_string(type));
	}
	return "";
}

void Parser::addFunction(funcPtr func,int level){
	functions.emplace(level, func);
}

Node* Parser::parse(Lexer& lexer){
	addNodeType("Prog");
	addNodeType("Null");
	addNodeType("Linker");
	Node* AST = new Node(NodeType("Prog"));
	AST->left = prs(lexer);
	return AST; // or return a meaningful result if applicable
}
Node* Parser::prs(Lexer& lexer){
	Node* node = new Node(NodeType("Linker"));
	node->right = new Node(NodeType("Null"));
	bool functionExecuted = false;
	for (auto it = functions.begin(); it != functions.end(); ++it) {
		Node* result = it->second(lexer,*this);
		if (result != nullptr) {
			functionExecuted = true;
			node->left = result;
			// If a function returns a non-null value, restart the loop
			break;
		}
	}
	if (!functionExecuted) {
		// If no functions executed successfully, return null
		return nullptr;
	}
	
	node->middle = prs(lexer);
	return node;
}

void Parser::consume() {
	index++;
	return;
}
Token& Parser::current(int offset){
	return tokens[index + offset];
}




*/
