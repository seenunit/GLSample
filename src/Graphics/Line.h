#pragma once


// CLine command target

class CLine : public CGeomEntity
{
	DECLARE_SERIAL(CLine)

private:
	CGeomPoint m_dptStart;
	CGeomPoint m_dptEnd;

	//CPoint m_dptStart;
	//CPoint m_dptEnd;
public:
	CLine( );
	CLine( CGeomPoint/*CPoint*/ ptFrom, CGeomPoint/*CPoint*/ ptTo );	
	virtual ~CLine();
public:
	void Draw();
    void DrawLine(CDC *pDC, Matrix &matTransf);
	virtual void Serialize(CArchive &ar);

	CGeomPoint GetStartPoint();
	CGeomPoint GetEndPoint();
	void ModifyLine( CGeomPoint ptFrom, CGeomPoint ptTo );
};
