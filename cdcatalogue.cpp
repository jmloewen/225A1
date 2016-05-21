// File:        cdcatalogue.cpp
// Author:      Jonathan Loewen
// Date:        2016-01-17
// Description: Implementation of a CDCatalogue class to be used for CMPT 225 Assignment #1

//January 12th: principal Coding.
//January 15th: Annotations & formatting.
//January 17th: Final bugfixing & formatting.

#include <iostream>
#include "cdcatalogue.h"
#include "cd.h"
#include <string>
using namespace std;

//Default Constructor.  
//Pre: Needs enough Memory for a CDCatalogue.  Post: CDCatalogue object instantiated.
CDCatalogue::CDCatalogue()
{
	maxsize = 4;
	numcds = 0;
	cds = new CD[maxsize];
}

//Parameterized Constructor, Requires a CDCatalogue object. Assigns the parameters of the sent CDCatalogue object and stores them, then calls the copyarray function.
//Pre: Needs enough Memory for a CDCatalogue.  Post: CDCatalogue object instantiated.  Required: A CDCatalogue object.
CDCatalogue::CDCatalogue(const CDCatalogue& cat)
{	
	//There are two ways to approach the double - My parameterized constructor could double maxsize itself, but I chose instead to do that in the Insert function.
	//my rationale for this is that you should be able to create a copy of a CDCatalogue without doubling the maximum size.
	maxsize = cat.maxsize;
	numcds = cat.numcds;
	cds = new CD[cat.maxsize];
	CopyArray(cat);
}		

//Copies the CDs in the sent catalogue and stores them in this object.
//Pre: Needs an existing CDCatalogue object.  Post: objects in this object's cds array now contains the values in cat's cds.  Required: A CDCatalogue object.
void CDCatalogue::CopyArray(const CDCatalogue& cat)
{
	for (int i = 0; i < cat.Count(); i++)
	{
		cds[i] = cat.cds[i];
	}
}


//Destructor of the CDCatalogue.  Only deletes the cdarray if there exists a cd in the array.
//Pre: A CDCatalogue object must exist.  Post: The CDCatalogue object has been destroyed with its internal array.
CDCatalogue::~CDCatalogue()
{
	if (numcds > 0)
	{
		delete[] cds;
	}
}

//Inserts a CD into the Catalogue's CD Array if the CD does not already exist within the catalogue.
//Pre: A Catalogue and a CD to be inserted.  Post: The cd is inserted if not a duplicate, returns the status of the insert (T or F)  Required: A CD to be inserted.
bool CDCatalogue::Insert(CD disc)
{
	//if our number of cds is less than our maximum catalogue size, enter.
	if (numcds < maxsize)
	{
		if (Find(disc) != -1)
		{
			return false;
		}
		//The CD does not yet exist in our collection.  Add it, increment numcds, and return true.
		cds[numcds] = disc;
		numcds++;
		return true;
	}
	else //therefore numcds is equal to maxsize, so maxsize must be doubled.
	{
		//if exists in the library, return.
		if (Find(disc) != -1)
		{
			return false;
		}

		//double the available size and create a new Catalogue of this size.
		maxsize = maxsize * 2;
		CDCatalogue *newCatalogue = new CDCatalogue(*this);
		
		//append the new cd, increment numcds, then assign the current cds to the new catalogue's position and return.
		newCatalogue->cds[numcds] = disc;
		numcds++;
		cds = newCatalogue->cds;
		return true;
	}
}

//This function removes a CD from the cd collection object's cds array.
//Pre: A CD to be removed.  Post: Either the CD has been removed, or the CD never existed in the library.  Returns the status (T/F of the removal.
//Required: A CD to be removed.
bool CDCatalogue::Remove(CD disc)
{
	//if there's nothing to remove, we don't remove anything.
	if (numcds == 0)
	{
		return false;
	}
	//get the value of Find.
	int findVal = Find(disc);
	
	//if the value of Find is greater than or equal to 0, we found a cd to remove.  Remove the cd.  Otherwise, return false.
	if (findVal >= 0)
	{
		cds[findVal] = cds[numcds-1];
		numcds--;
		return true;
	}
	return false;
}

//Find searches for the index of the stated cd 'disc' and returns it.  If it does not find this disc, it returns -1.
//Pre: A disc to find.  Post: Returns the index of the sent disc, or -1 if not found.  Required: A disc to find.
int CDCatalogue::Find(CD disc) const
{
	if (numcds > 0)
	{
		//iteratively search through every cd in our array of cds.
		for (int i = 0; i < Count(); i++)
		{
			if (cds[i].GetAlbum() == disc.GetAlbum() && cds[i].GetArtist() == disc.GetArtist())
			{
				return i;
			}
		}
	}
	return -1;
}

//This function searches for all CDs with a given artist and removes them from the CD array.
//Pre: A string containing an artist, whose CD's we're removing from our collection.
//Post: All cds from the artist of 'dontlikeanymore' have been removed.
//Required: An artist to remove from the catalogue.
bool CDCatalogue::Boycott(string dontlikeanymore)
{
	//a flag that flips true if we find something to remove.  This value is later returned to the caller.
	bool successflag = false;
	for (int i = 0; i < numcds; i++)
	{
		//if the current cd's artist matches the sent string, remove the cd from the collection and decrement i.
		//i is decremented so that we check the current index again - in case the previous end value is also the hated artist.
		if (cds[i].GetArtist() == dontlikeanymore)
		{
			cds[i] = cds[numcds-1];
			i--;
			numcds--;
			successflag = true;
		}
	}
	return successflag;
}

//this function returns the value of numcds to the caller.
//Pre: The CDCatalogue must exist.  Post: The number of cds in the collection is returned.
int CDCatalogue::Count() const
{
	return numcds;
}

//this function returns a CDCatalogue object containing all values in both the existing catalogue and the sent cat, with no duplicates.
//Pre: The CDCatalogue object must exist, and cat must also exist.  Post: Returns a Join of this.cds and cat.cds.  Required: valid this catalogue and cat.cds
CDCatalogue CDCatalogue::Join(const CDCatalogue& cat) const
{
	CDCatalogue *newcat = new CDCatalogue(cat); //take all the values from cat into the new catalogue.

	//insert values of cds into newcat.
	if (Count() > 0)
	{
		for (int i = 0; i < numcds; i++)
		{
			newcat->Insert(cds[i]);
		}
	}

	return *newcat;
}

//common finds the set of CDs that are in both 'this''s cds AND in 'cat''s cds.  It creates a CDCatalogue object of these cds and returns it to the caller.
//Pre: this cdcatalogue.cds and cat.cds must exist.  Post: Returns a catalogue of this.cds and cat.cds values.  Requires: this.cds and cat.cds must exist.
CDCatalogue CDCatalogue::Common(const CDCatalogue& cat) const
{
	CDCatalogue newcat;

	//add values of cds to newcat only if they also exist in cat.
	if (cat.Count() != 0)
	{
		for (int i = 0; i < cat.Count(); i++)
		{
			if (Find(cat.cds[i]) != -1)
			{
				newcat.Insert(cat.cds[i]);
			}
		}
	}
	return newcat;
}

//Split returns a CDCatalogue object consisting of values that are in this.cds but not also in cat.cds (Set A - Set B).  Note: Not the Symmetric Difference.
//Pre: This cdcatalogue and cat cdcatalogue must exist.  Post: returns the values in this.cds that are not also in cat.cds.  Requires: this cdcatalogue and cat cdcatalogue must both exist.
CDCatalogue CDCatalogue::Split(const CDCatalogue& cat) const
{
	CDCatalogue newcat;
	
	//first, insert all values that exist in cds to newcat.
	for (int i = 0; i < numcds; i++)
	{
		newcat.Insert(cds[i]);
	}

	if (cat.Count() > 0)
	{
		//remove all cat cds.
		for (int i = 0; i < cat.Count(); i++)
		{
			newcat.Remove(cat.cds[i]);
		}
	}
	return newcat;
}