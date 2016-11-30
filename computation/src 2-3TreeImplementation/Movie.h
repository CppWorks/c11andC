#pragma once

#define MAX_TITLE_SIZE 1024
#include <string.h>
#include <iostream>

class CMovie
{
public:
    CMovie(void);
	CMovie(char *sTitle);
	CMovie(char *sTitle, int iReleaseYear);
    virtual ~CMovie(void);

    inline const char* const getTitle(void) const 
    { 
        return m_sTitle;
    };

	inline int getReleaseYear(void) const 
    { 
        return m_iReleaseYear;
    };

    inline bool operator< (const CMovie& rCMovie) const
    {
        return (strcmp(this->getTitle(), rCMovie.getTitle()) < 0);
    };  

   /** @brief output operator overloading.  
    */
    friend std::ostream& operator<< (std::ostream& out, CMovie& rCMovie)
    {
        out <<rCMovie.getTitle()<<";"<<rCMovie.getReleaseYear(); 
        return out;
    }; 

private:
    char m_sTitle[MAX_TITLE_SIZE + 1];
    int m_iReleaseYear;  
};

