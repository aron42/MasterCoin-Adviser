#include "BinaryData.h"
#include "BtcUtils.h"

////////////////////////////////////////////////////////////////////////////////
BinaryData::BinaryData(BinaryDataRef const & bdRef) 
{ 
   copyFrom(bdRef.getPtr(), bdRef.getSize());
}


////////////////////////////////////////////////////////////////////////////////
inline void BinaryData::copyFrom(BinaryDataRef const & bdr)
{
   copyFrom( bdr.getPtr(), bdr.getSize() );
}


////////////////////////////////////////////////////////////////////////////////
BinaryDataRef BinaryData::getRef(void) const
{
   return BinaryDataRef(getPtr(), nBytes_);
}



////////////////////////////////////////////////////////////////////////////////
inline BinaryData & BinaryData::append(BinaryDataRef const & bd2)
{
   data_.insert(data_.end(), bd2.getPtr(), bd2.getPtr()+bd2.getSize());
   nBytes_ += bd2.getSize();
   return (*this);
}



////////////////////////////////////////////////////////////////////////////////
int32_t BinaryData::find(BinaryDataRef const & matchStr, uint32_t startPos)
{
   int32_t finalAnswer = -1;
   for(int32_t i=startPos; i<=(int32_t)nBytes_-(int32_t)matchStr.getSize(); i++)
   {
      if(matchStr[0] != data_[i])
         continue;

      for(uint32_t j=0; j<matchStr.getSize(); j++)
      {
         if(matchStr[j] != data_[i+j])
            break;

         // If we are at this instruction and is the last index, it's a match
         if(j==matchStr.getSize()-1)
            finalAnswer = i;
      }

      if(finalAnswer != -1)
         break;
   }

   return finalAnswer;
}


////////////////////////////////////////////////////////////////////////////////
int32_t BinaryData::find(BinaryData const & matchStr, uint32_t startPos)
{
   BinaryDataRef bdrmatch(matchStr);
   return find(bdrmatch, startPos);
}


////////////////////////////////////////////////////////////////////////////////
bool BinaryData::contains(BinaryData const & matchStr, uint32_t startPos)
{
   return (find(matchStr, startPos) != -1);
}

////////////////////////////////////////////////////////////////////////////////
bool BinaryData::contains(BinaryDataRef const & matchStr, uint32_t startPos)
{
   return (find(matchStr, startPos) != -1);
}



/////////////////////////////////////////////////////////////////////////////
bool BinaryData::startsWith(BinaryDataRef const & matchStr) const
{
   if(matchStr.getSize() > nBytes_)
      return false;

   for(uint32_t i=0; i<matchStr.getSize(); i++)
      if(matchStr[i] != (*this)[i])
         return false;

   return true;
}

/////////////////////////////////////////////////////////////////////////////
bool BinaryData::startsWith(BinaryData const & matchStr) const
{
   if(matchStr.getSize() > nBytes_)
      return false;

   for(uint32_t i=0; i<matchStr.getSize(); i++)
      if(matchStr[i] != (*this)[i])
         return false;

   return true;
}

/////////////////////////////////////////////////////////////////////////////
bool BinaryData::endsWith(BinaryDataRef const & matchStr) const
{
   uint32_t sz = matchStr.getSize();
   if(sz > nBytes_)
      return false;
   
   for(uint32_t i=0; i<sz; i++)
      if(matchStr[sz-(i+1)] != (*this)[nBytes_-(i+1)])
         return false;

   return true;
}
/////////////////////////////////////////////////////////////////////////////
bool BinaryData::endsWith(BinaryData const & matchStr) const
{
   uint32_t sz = matchStr.getSize();
   if(sz > nBytes_)
      return false;
   
   for(uint32_t i=0; i<sz; i++)
      if(matchStr[sz-(i+1)] != (*this)[nBytes_-(i+1)])
         return false;

   return true;
}

/////////////////////////////////////////////////////////////////////////////
BinaryDataRef BinaryData::getSliceRef(uint32_t start_pos, uint32_t nChar) const
{
   if(start_pos < 0) 
      start_pos = nBytes_ + start_pos;

   if(start_pos + nChar >= nBytes_)
   {
      cerr << "getSliceRef: Invalid BinaryData access" << endl;
      return BinaryDataRef();
   }
   return BinaryDataRef( getPtr()+start_pos, nChar);
}

/////////////////////////////////////////////////////////////////////////////
BinaryData BinaryData::getSliceCopy(uint32_t start_pos, uint32_t nChar) const
{
   if(start_pos < 0) 
      start_pos = nBytes_ + start_pos;

   if(start_pos + nChar >= nBytes_)
   {
      cerr << "getSliceCopy: Invalid BinaryData access" << endl;
      return BinaryData();
   }
   return BinaryData(getPtr()+start_pos, nChar);
}



/////////////////////////////////////////////////////////////////////////////
uint64_t BinaryReader::get_var_int(uint8_t* nRead)
{
   uint32_t nBytes;
   uint64_t varInt = BtcUtils::readVarInt( bdStr_.getPtr() + pos_, &nBytes);
   if(nRead != NULL)
      *nRead = nBytes;
   pos_ += nBytes;
   return varInt;
}

/////////////////////////////////////////////////////////////////////////////
uint64_t BinaryRefReader::get_var_int(uint8_t* nRead)
{
   uint32_t nBytes;
   uint64_t varInt = BtcUtils::readVarInt( bdRef_.getPtr() + pos_, &nBytes);
   if(nRead != NULL)
      *nRead = nBytes;
   pos_ += nBytes;
   return varInt;
}


/////////////////////////////////////////////////////////////////////////////
inline bool BinaryData::operator==(BinaryDataRef const & bd2) const
{
   if(nBytes_ != bd2.getSize())
      return false;
   for(unsigned int i=0; i<nBytes_; i++)
      if( data_[i] != bd2[i])
         return false;
   return true;
}




