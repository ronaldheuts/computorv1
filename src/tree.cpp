#include "tree.h"

/* Nodes */

BinaryExpr::BinaryExpr(Token::Kind k, std::unique_ptr<node_t>& l,
                       std::unique_ptr<node_t>& r)
    : oper{k}, left{std::move(l)}, right{std::move(r)} {}

UnaryExpr::UnaryExpr(Token::Kind k, std::unique_ptr<node_t>& c)
    : oper{k}, child{std::move(c)} {}

/* Tree */

Tree::Tree() : root{} {}

void Tree::setRoot(std::unique_ptr<node_t> expr) { root = std::move(expr); }

Tree::node_t& Tree::getRoot() const { return *root; }
