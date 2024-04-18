#include <wx/wx.h>
#include "Node.h"


class TreeControl : public wxScrolledWindow {
public:
	Node* root = nullptr;
	float zoomFactor = 1.0f;

	TreeControl(wxWindow* parent, Node* root) : wxScrolledWindow(parent) {
		SetScrollRate(10, 10);
		SetVirtualSize(2000, 1000);
		this->root = root;
		Bind(wxEVT_CHAR, &TreeControl::OnChar, this);
	}

	void zoomIn() {
		zoomFactor *= 1.1f;
		Refresh();
	}

	void zoomOut() {
		zoomFactor /= 1.1f;
		Refresh();
	}

protected:
	void OnDraw(wxDC& dc) override {
		if (!root) return;

		dc.SetUserScale(zoomFactor, zoomFactor);
		drawNode(dc, root, 1000, 45);
	}

	void OnChar(wxKeyEvent& event) {
		char letterPressed = event.GetUnicodeKey();
		if (letterPressed == '+' || letterPressed == '=')
			zoomIn();
		else if (letterPressed == '-')
			zoomOut();
	}

	void drawNode(wxDC& dc, Node* node, int x, int y, float depth = 0) {
		dc.DrawCircle(wxPoint(x, y), 20);
		dc.DrawText(!node->left && !node->right ? node->letters : std::to_string(node->freq), wxPoint(x - 5, y - 5));

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

};
