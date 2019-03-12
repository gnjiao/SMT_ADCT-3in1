#pragma once
#include <afxcmn.h>
class CMyListCtrl :
	public CListCtrl
{
public:
	CMyListCtrl();
	~CMyListCtrl();

	void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

	void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);

	void SetRowHeigt(int nHeight);

	void DrawItem(LPDRAWITEMSTRUCT lpMeasureItemStruct);

	int m_nRowHeight;

};

