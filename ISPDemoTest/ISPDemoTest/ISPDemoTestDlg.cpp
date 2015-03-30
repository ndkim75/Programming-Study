
// ISPDemoTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ISPDemoTest.h"
#include "ISPDemoTestDlg.h"
#include "afxdialogex.h"

#include <io.h>
#include "EXIF.H"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CISPDemoTestDlg dialog



CISPDemoTestDlg::CISPDemoTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CISPDemoTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pImageRaw = NULL;
}

void CISPDemoTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CISPDemoTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_FILE_OPEN_RAW, &CISPDemoTestDlg::OnBnClickedFileOpenRaw)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CONVERT_BAYER10, &CISPDemoTestDlg::OnBnClickedConvertBayer10bit)
	ON_BN_CLICKED(IDC_BUTTON1, &CISPDemoTestDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CISPDemoTestDlg message handlers

BOOL CISPDemoTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CISPDemoTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CISPDemoTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CISPDemoTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CISPDemoTestDlg::OnBnClickedFileOpenRaw()
{
	CFileDialog dlg( TRUE, NULL,NULL, 0, _T("Raw File(*.raw)|*.raw|") );
	if (dlg.DoModal() == IDOK)
	{
		if(m_pImageRaw!=NULL)
		{
			delete []m_pImageRaw;
			m_pImageRaw = NULL;
		}

		int image_size = 2576*1932;
		m_pImageRaw = new BYTE[image_size/4+image_size];

		CString strFileName;
		strFileName.Format("%s", dlg.GetPathName());
		FILE *fp;
		fp=fopen(strFileName, "rb");
		size_t result = fread (m_pImageRaw,1,image_size/4+image_size,fp);
		fclose(fp);
	}
}


void CISPDemoTestDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	if(m_pImageRaw!=NULL)
	{
		delete []m_pImageRaw;
		m_pImageRaw = NULL;
	}

}


void CISPDemoTestDlg::OnBnClickedConvertBayer10bit()
{
	if(m_pImageRaw == NULL) return;

	int image_size = 2576*1932;
	BYTE *pImageBayer10 = new BYTE[image_size*2];

	int i,k;
	int nByteSize;

	nByteSize = image_size*5/4;

	k=0;
	for(i=0; i<nByteSize ; i+=5)
	{
		pImageBayer10[k+1] = (m_pImageRaw[i] & 0xC0) >> 6;
		pImageBayer10[k+0] = ((m_pImageRaw[i] & 0x3F) << 2) +  ((m_pImageRaw[i + 4] & 0xC0) >> 6);

		pImageBayer10[k+3] = (m_pImageRaw[i + 1] & 0xC0) >> 6;
		pImageBayer10[k+2] = ((m_pImageRaw[i + 1] & 0x3F) << 2) +  ((m_pImageRaw[i + 4] & 0x30) >> 4);

		pImageBayer10[k+5] = (m_pImageRaw[i + 2] & 0xC0) >> 6;
		pImageBayer10[k+4] = ((m_pImageRaw[i + 2] & 0x3F) << 2) +  ((m_pImageRaw[i + 4] & 0x0C) >> 2);

		pImageBayer10[k+7] = (m_pImageRaw[i + 3] & 0xC0) >> 6;
		pImageBayer10[k+6] = ((m_pImageRaw[i + 3] & 0x3F) << 2) +  ((m_pImageRaw[i + 4] & 0x03)) ;

		k += 8;
	}

	BYTE *pImageBayer8 = new BYTE[image_size];

	WORD *pValue;
	for(i=0;i<image_size;i++)
	{
		pValue = (WORD *)(pImageBayer10+i*2);
		WORD nValue = (*pValue)>>2;
		pImageBayer8[i] = (*pValue)>>2;
	}

	IplImage *pIplImage = NULL;
	pIplImage = cvCreateImage(cvSize(2576, 1932), IPL_DEPTH_8U, 1);

	IplImage *pIplImageDest = NULL;
	pIplImageDest = cvCreateImage(cvSize(2576, 1932), IPL_DEPTH_8U, 3);

	IplImage *pIplImageDestG = NULL;
	pIplImageDestG = cvCreateImage(cvSize(2576, 1932), IPL_DEPTH_8U, 1);

	char *pTemp = pIplImage->imageData;
	pIplImage->imageData = (char *)(pImageBayer8);
	cvZero(pIplImageDest);
	cvZero(pIplImageDestG);

	LARGE_INTEGER iPerfFreq;
	LARGE_INTEGER iPerfStart;
	LARGE_INTEGER iPerfEnd;
	QueryPerformanceFrequency(&iPerfFreq);
	QueryPerformanceCounter(&iPerfStart);

	int width = 2576;
	int height = 1932;
	WORD r=0, g=0, b=0;
	WORD *pwImageBayer10 = (WORD *)pImageBayer10;
	for(int y=1;y<height-1;y++)
	{
		for(int x=1;x<width-1;x++)
		{
			if(y%2==0)
			{
				// GR 라인
				if(x%2==0)
				{

					r = pImageBayer8[y*width+x];
					b = (pImageBayer8[(y-1)*width+x]+ pImageBayer8[(y+1)*width+x])/2;
					g = (pImageBayer8[y*width+x-1] + pImageBayer8[y*width+x+1])/2;

// 					r = pImageBayer8[y*width+x];
//  					b = (pImageBayer8[y*width+x-1] + pImageBayer8[y*width+x+1] + pImageBayer8[(y-1)*width+x]+ pImageBayer8[(y+1)*width+x])/4;
//  					r = (pImageBayer8[(y-1)*width+x-1] + pImageBayer8[(y-1)*width+x+1] + pImageBayer8[(y+1)*width+x-1]+ pImageBayer8[(y+1)*width+x+1])/4;

// 					b = (pImageBayer8[y*width+x-1] + pImageBayer8[y*width+x+1])/2;
// 					r = pImageBayer8[y*width +x];
// 					g = (pImageBayer8[(y-1)*width+x]+ pImageBayer8[(y+1)*width+x])/2;

					pIplImageDest->imageData[y*width*3 + x*3    ] = char(r);
					pIplImageDest->imageData[y*width*3 + x*3 + 1] = char(r);
					pIplImageDest->imageData[y*width*3 + x*3 + 2] = char(r);

				}
				else
				{
// 					g = (pImageBayer8[y*width+x-1] + pImageBayer8[y*width+x+1])/2;
// 					b = pImageBayer8[y*width +x];
// 					g = (pImageBayer8[(y-1)*width+x]+ pImageBayer8[(y+1)*width+x])/2;
				}
			}
			else
			{
// 				// BG 라인
// 				if(x%2==0)
// 				{
// 					// B 기준
// 					r = (pImageBayer8[(y-1)*width+x-1] + pImageBayer8[(y-1)*width+x+1] + pImageBayer8[(y+1)*width+x-1]+ pImageBayer8[(y+1)*width+x+1])/4;
// 					g = (pImageBayer8[y*width+x-1] + pImageBayer8[y*width+x+1] + pImageBayer8[(y-1)*width+x]+ pImageBayer8[(y+1)*width+x])/4;;
// 					b = pImageBayer8[y*width+x];
// 				}
// 				else
// 				{
// 					// G 기준
// 					r = (pImageBayer8[(y-1)*width+x]+ pImageBayer8[(y+1)*width+x])/2;
// 					g = pImageBayer8[y*width +x];
// 					b = (pImageBayer8[y*width+x-1] + pImageBayer8[y*width+x+1])/2;
// 				}

// 				// RG 라인
// 				if(x%2==0)
// 				{
// 					// R 기준
// 					b = (pImageBayer8[(y-1)*width+x-1] + pImageBayer8[(y-1)*width+x+1] + pImageBayer8[(y+1)*width+x-1]+ pImageBayer8[(y+1)*width+x+1])/4;
// 					g = (pImageBayer8[y*width+x-1] + pImageBayer8[y*width+x+1] + pImageBayer8[(y-1)*width+x]+ pImageBayer8[(y+1)*width+x])/4;
// 					r = pImageBayer8[y*width+x];
// 				}
// 				else
// 				{
// 					// G 기준
// 					b = (pImageBayer8[(y-1)*width+x]+ pImageBayer8[(y+1)*width+x])/2;
// 					g = pImageBayer8[y*width+x];
// 					r = (pImageBayer8[y*width+x-1] + pImageBayer8[y*width+x+1])/2;
// 				}
			}
// 			pIplImageDest->imageData[y*width*3 + x*3    ] = char(b);
// 			pIplImageDest->imageData[y*width*3 + x*3 + 1] = char(g);
// 			pIplImageDest->imageData[y*width*3 + x*3 + 2] = char(r);
//			pIplImageDestG->imageData[y*width + x] = char(g);/*char(16+0.183*r+0.614*g+0.062*b)*/;
		}
	}
	QueryPerformanceCounter(&iPerfEnd);
	CString strText;
	strText.Format("%d", ((DWORD)(((iPerfEnd.QuadPart - iPerfStart.QuadPart)*1000)/iPerfFreq.QuadPart)));
//	AfxMessageBox(strText);
//	cvCvtColor(pIplImage, pIplImageDest, CV_BayerGB2BGR_VNG);
	
	cvSaveImage("d:\\Bayer8bit.bmp", pIplImage);
	cvSaveImage("d:\\RGB8bit.bmp", pIplImageDest);
//	cvSaveImage("d:\\Green8bit.bmp", pIplImageDestG);
	pIplImage->imageData = pTemp;

	cvReleaseImage(&pIplImage);
	cvReleaseImage(&pIplImageDest);
	cvReleaseImage(&pIplImageDestG);

	delete [] pImageBayer10;
	delete [] pImageBayer8;
}


void CISPDemoTestDlg::OnBnClickedButton1()
{
	EXIFINFO exifinfo;
	FILE* hFile=fopen("D:\\OTP2.jpg","rb");
	if (hFile){
		memset(&exifinfo,0,sizeof(EXIFINFO));
		Cexif exif(&exifinfo);
		exif.DecodeExif(hFile);
		fclose(hFile);

 		if (exifinfo.IsExif)
		{
			// 비교
			// exifinfo.ImageUniqueID == ???
		}
		else
		{
			// error
		}
	}
}
