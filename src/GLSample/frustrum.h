#ifndef FRUSTRUM_H
#define FRUSTRUM_H

extern "C" {
	void FSTART( int *);
	void FABORT( int *);
	void FSTOP( int *);
	void FMALLO( int *, char **, int *);
	void FMFREE( int *, char **, int *);
	void GOSGMT( const int *, const int *, const int *, const int *,
						const double *, const int *, const int *, int *);
	void GOOPSG( const int *, const int *, const int *, const int *,
						const double *, const int *, const int *, int *);
	void GOCLSG( const int *, const int *, const int *, const int *,
						const double *, const int *, const int *, int *);
	void GOPIXL( const int *, const double *, const int *, const int *,
						int *);
	void GOOPPX( const int *, const double *, const int *, const int *,
						int *);
	void GOCLPX( const int *, const double *, const int *, const int *,
						int *);
	void FFOPRD( const int *, const int *, const char *, const int *,
						const int *, int *, int *);
	void FFOPWR( const int *, const int *, const char *, const int *,
						const char *, const int *, int *, int *);
	void FFCLOS( const int *, const int *, const int *, int *);
	void FFREAD( const int *, const int *, const int *, char *, int *,
						int *);
	void FFWRIT( const int *, const int *, const int *, const char *,
						int *);
	void FFOPRB( const int *, const int *, const int *, int *, int *,
						int *);
	void FFSEEK( const int *, const int *, const int *, int *);
	void FFTELL( const int *, const int *, int *, int *);
	void FGCRCU( const char *, int *, int *, int *, int *, double *,
						int *, double *, int *);
	void FGCRSU( const char *, int *, int *, int *, int *, double *,
						int *, double *, int *);
	void FGEVCU( int *, double *, double *, double *, int *,
						double *, int *);
	void FGEVSU( int *, double *, double *, double *, double *,
						int *, int *, int *, double *, int *);
	void FGPRCU( int *, double *, double *, double *, int *, int *);
	void FGPRSU( int *, double *, double *, double *, int *, int *);
}

#endif