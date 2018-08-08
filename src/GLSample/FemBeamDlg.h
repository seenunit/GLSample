#pragma once


// CFemBeamDlg dialog

class CFemBeamDlg : public CDialog
{
	DECLARE_DYNAMIC(CFemBeamDlg)

    CGLSampleDoc *m_pCurrentDoc;
public:
	CFemBeamDlg(CGLSampleDoc *pDoc, CWnd* pParent = NULL);   // standard constructor
	virtual ~CFemBeamDlg();

// Dialog Data
	enum { IDD = IDD_FEM_BEAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedFemCancel();
    afx_msg void OnBnClickedFemOk();
    // Beam length
    double m_dBeamLength;
    // Number of nodes on beam
    int m_iBeamNodes;
    // Youngs moduli of the beam
    double m_bBeamModuli;
    // Elemental area of the beam
    double m_dBeamArea;
};
