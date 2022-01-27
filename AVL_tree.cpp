#include <iostream>
#include <clocale>

struct node {
	int data, bfactor;
	node* left, * right;
}*root;

node* deleteTree(node* root) {
	if (root) {
		deleteTree(root->left);
		deleteTree(root->right);
		delete root;
	}
	return nullptr;
}

void RNL(node* p) {
	if (p) {
		RNL(p->right);
		std::cout << p->data << " ";
		RNL(p->left);
	}
}

void outputTree(int x, node* proot) {
	if (proot) {
		std::cout << x << ':' << proot->data << " ";
		outputTree(proot->data, proot->left);
		outputTree(proot->data, proot->right);
	}
}

void height(int* h, node* p) {
	if (p) {
		*h = *h + 1;
		height(h, p->left);
		height(h, p->right);
		if (p->left && p->right) *h = *h - 1;
	}
}

void bfactorOutput(node* p) {
	if (p) {
		std::cout << p->data << ':' << p->bfactor << " ";
		bfactorOutput(p->left);
		bfactorOutput(p->right);
	}
}

void bfactorCreate(node* p) {
	if (p) {
		int hL = 0, hR = 0;
		height(&hL, p->left);
		height(&hR, p->right);
		p->bfactor = hL - hR;
		bfactorCreate(p->left);
		bfactorCreate(p->right);
	}
}

void makeTree(int x, node** proot) {
	node* p = *proot;
	if (!p) {
		p = new node;
		p->data = x;
		p->right = p->left = nullptr;
		*proot = p;
	}
	else if (p->data > x) makeTree(x, &p->left);
	else makeTree(x, &p->right);
}

node* rR(node* p) {
	node* p1 = p->left;
	int hL = 0, hR = 0;
	height(&hL, p1->left);
	height(&hR, p1->right);
	if (hL >= hR) {
		p->left = p1->right;
		p1->right = p;
		return p1;
	}
	else {
		node* p2 = p1->right;
		p->left = p2->right;
		p1->right = p2->left;
		p2->right = p;
		p2->left = p1;
		return p2;
	}
}

node* lR(node* p) {
	node* p1 = p->right;
	int hL = 0, hR = 0;
	height(&hL, p1->left);
	height(&hR, p1->right);
	if (hR >= hL) {
		p->right = p1->left;
		p1->left = p;
		return p1;
	}
	else {
		node* p2 = p1->left;
		p->right = p2->left;
		p1->left = p2->right;
		p2->left = p;
		p2->right = p1;
		return p2;
	}
}

void balance(bool* d, node* p1, node* p) {
	bfactorCreate(root);
	if (p) {
		if (p->bfactor >= 2) {
			if (p1->left == p) p1->left = rR(p);
			else if (p1->right == p) p1->right = rR(p);
			else root = rR(p);
			*d = true;
		}
		else if (p->bfactor <= -2) {
			if (p1->left == p) p1->left = lR(p);
			else if (p1->right == p) p1->right = lR(p);
			else root = lR(p);
			*d = true;
		}
		balance(d, p, p->left);
		balance(d, p, p->right);
	}
}

int main() {
	std::setlocale(LC_CTYPE, "Russian");
	int n, x;
	bool d = true;
	root = nullptr;
	std::cout << "Введите кол-во эл-ов дерева: ";
	std::cin >> n;
	std::cin.ignore(INT_MAX, '\n');
	if ((!std::cin.good()) || (n < 1) || (std::cin.gcount() > 1)) {
		std::cout << "Ошибка: вы ввели не натуральное число. Завершение программы";
		exit(1);
	}
	for (int i = 0; i < n; i++) {
		std::cout << "Введите " << i + 1 << " эл-т дерева: ";
		std::cin >> x;
		std::cin.ignore(INT_MAX, '\n');
		if (!std::cin.good() || (std::cin.gcount() > 1)) {
			std::cout << "Ошибка: вы ввели не целое число. Завершение программы";
			exit(1);
		}
		makeTree(x, &root);
	}
	bfactorCreate(root);
	std::cout << std::endl << "Обычный вывод дерева: ";
	outputTree(0, root);
	std::cout << std::endl << "Вывод коэффициента сбалансированности эл-ов дерева: ";
	bfactorOutput(root);
	while (d) {
		d = false;
		balance(&d, root, root);
	}
	std::cout << std::endl << "Балансировка и обычный вывод дерева: ";
	outputTree(0, root);
	std::cout << std::endl << "Вывод коэффициента сбалансированности эл-ов дерева: ";
	bfactorOutput(root);
	std::cout << std::endl << "RNL вывод дерева: ";
	RNL(root);
	root = deleteTree(root);
	if (!root) std::cout << std::endl << std::endl << "Дерево удалено.";
	else std::cout << std::endl << std::endl << "Произошла ошибка при удалении дерева. Вероятно, память очищена не полностью.";
	return 0;
}