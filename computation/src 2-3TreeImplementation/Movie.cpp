#include "stdafx.h"
#include "Movie.h"
#include <iostream>

CMovie::CMovie(void)
{
   m_sTitle[0] = '\0';
   m_iReleaseYear = 0;
}

CMovie::CMovie(char *sTitle)
{
    strcpy_s(m_sTitle,sTitle);
    m_iReleaseYear = 0;
}

CMovie::CMovie(char *sTitle, int iReleaseYear)
{
    strcpy_s(m_sTitle,sTitle);
	m_iReleaseYear = iReleaseYear;
}

CMovie::~CMovie(void)
{
}
