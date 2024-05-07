#include "TreeControl.h"


TreeControl::TreeControl(wxWindow* parent, Node* root) : wxScrolledWindow(parent) {
	SetScrollRate(10, 10);
	SetVirtualSize(2000, 1000);
	this->root = root;
	Bind(wxEVT_CHAR, &TreeControl::OnChar, this);
}

void TreeControl::zoomIn() {
	zoomFactor *= 1.1f;
	Refresh();
}

void TreeControl::zoomOut() {
	zoomFactor /= 1.1f;
	Refresh();
}

void TreeControl::OnDraw(wxDC& dc) {
	if (!root) return;

	dc.SetUserScale(zoomFactor, zoomFactor);
	drawNode(dc, root, 1000, 45);
}

void TreeControl::OnChar(wxKeyEvent& event) {
	char letterPressed = event.GetUnicodeKey();
	if (letterPressed == '+' || letterPressed == '=')
		zoomIn();
	else if (letterPressed == '-')
		zoomOut();
}

void TreeControl::drawNode(wxDC& dc, Node* node, int x, int y, float depth) {
	dc.DrawCircle(wxPoint(x, y), 20);
	dc.DrawText(node->letter ? std::to_string(node->letter) : std::to_string(node->freq), wxPoint(x - 5, y - 5));

	float childY = y + 100;
	float childXSpacing = 300 / (depth + 1);

	if (depth >= 4) return;

	if (node->left) {
		float childX = x - childXSpacing;
		dc.DrawLine(x, y + 20, childX, childY - 20);
		drawNode(dc, node->left, childX, childY, depth + 1);
	}

	if (node->right) {
		float childX = x + childXSpacing;
		dc.DrawLine(x, y + 20, childX, childY - 20);
		drawNode(dc, node->right, childX, childY, depth + 1);
	}
}