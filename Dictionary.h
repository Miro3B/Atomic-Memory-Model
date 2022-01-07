//
//
// Dictionary.h: interface for the Dictionary class.
//
//                   ATOMIC MEMORY MODEL - Implementation Example 'Phase Two'
//
// © Copyright 2007 - 2013 by Miroslav Bonchev Bonchev. All rights reserved.
//
//
//******************************************************************************************************


// Open Source License - The MIT License
//
//
// Atomic Memory Model © Copyright 2001 - 2013 by Miroslav Bonchev Bonchev.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
// associated  documentation files  (the "Software"),  to deal  in the Software without restriction,
// including  without  limitation the rights  to use,  copy,  modify,  merge,  publish,  distribute,
// sublicense,  and/or sell copies of the Software,  and to permit persons to  whom the  Software is
// furnished to do so, subject to the following conditions:
//
// The  above  copyright  notice  and  this  permission  notice  shall be  included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
// NOT  LIMITED  TO  THE  WARRANTIES  OF  MERCHANTABILITY,  FITNESS  FOR  A  PARTICULAR  PURPOSE AND 
// NONINFRINGEMENT.  IN NO EVENT SHALL  THE  AUTHORS  OR  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY,  WHETHER IN AN ACTION OF CONTRACT,  TORT OR OTHERWISE,  ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


//__________________________________________________________________________________________________
// This software is OSI Certified Open Source Software.
// OSI Certified is a certification mark of the Open Source Initiative.


#pragma once


#include "MMemory.h"
#include "MList.h"
#include "Pair.h"


class DictionaryException
{
public:
   enum Exception
   {
      ValueNotFound,
      LabelNotFound,
      DuplicateLabel
   };

private:
   Exception eException;

public:
   DictionaryException( Exception eException ) : eException( eException ) {}
   Exception GetException() const { return( eException ); }
};


template< class Label, class Value >
class Dictionary : protected MList< Pair< Label, Value > >
{
public:
   Dictionary()
   {
   }


   typedef Pair< Label, Value > typeMembers;


   Dictionary( Pair< Label, Value >* ptmHeadObject, ... )
   {
      va_list   listArg;
      va_start( listArg, ptmHeadObject );

      typeMembers* pObject( ptmHeadObject );

      while( NULL != pObject )
      {
         AddTail( pObject );

         pObject = va_arg( listArg, typeMembers* );
      }

      va_end( listArg );

      this->Accelerate();
   }


   virtual ~Dictionary()
   {
   }


   bool IsEmpty() const throw()
   {
      return( MList< Pair< Label, Value > >::IsEmpty() );
   }


   Dictionary< Label, Value >& AddTail( Pair< Label, Value >* pNewTail )
   {
      for( MLISTINDEX mliIndex = 0; mliIndex < this->GetCount(); mliIndex++ )
      {
         if( pNewTail->GetLabelRef() == MList< Pair< Label, Value > >::operator[]( mliIndex )->GetLabelRef() )
         {
            MASSERT( FALSE );

            throw( DictionaryException( DictionaryException::DuplicateLabel ) );
         }
      }

      MList< Pair< Label, Value > >::AddTail( pNewTail );

      return( *this );
   }


   Dictionary< Label, Value >& AddTail( const Label& objLabel, const Value& objValue )
   {
      for( MLISTINDEX mliIndex = 0; mliIndex < this->GetCount(); mliIndex++ )
      {
         if( objLabel == MList< Pair< Label, Value > >::operator[]( mliIndex )->GetLabelRef() )
         {
            MASSERT( FALSE );

            throw( DictionaryException( DictionaryException::DuplicateLabel ) );
         }
      }

      MList< Pair< Label, Value > >::AddTail( new Pair< Label, Value >( objLabel, objValue ) );

      return( *this );
   }


   Pair< Label, Value >* GetTail()
   {
      return( MList< Pair< Label, Value > >::GetTail() );
   }


   const Pair< Label, Value >* GetTail() const
   {
      return( MList< Pair< Label, Value > >::GetTail() );
   }


   Dictionary< Label, Value >& Add( Pair< Label, Value >* pNewTail )
   {
      for( MLISTINDEX mliIndex = 0; mliIndex < this->GetCount(); mliIndex++ )
      {
         if( pNewTail->GetLabelRef() == MList< Pair< Label, Value > >::operator[]( mliIndex )->GetLabelRef() )
         {
            MASSERT( FALSE );

            throw( DictionaryException( DictionaryException::DuplicateLabel ) );
         }
      }

      MList< Pair< Label, Value > >::AddMemberSorted( pNewTail, true );

      return( *this );
   }


   Dictionary< Label, Value >& Add( const Label& objLabel, const Value& objValue )
   {
      for( MLISTINDEX mliIndex = 0; mliIndex < this->GetCount(); mliIndex++ )
      {
         if( objLabel == MList< Pair< Label, Value > >::operator[]( mliIndex )->GetLabelRef() )
         {
            MASSERT( FALSE );

            throw( DictionaryException( DictionaryException::DuplicateLabel ) );
         }
      }

      MList< Pair< Label, Value > >::AddMemberSorted( new Pair< Label, Value >( objLabel, objValue, Pair< Label, Value >::sortByLabel ), true );

      return( *this );
   }


   template< class Comparator >
   Dictionary< Label, Value >& Add( const Comparator& objComparator, const Label& objLabel, const Value& objValue )
   {
      for( MLISTINDEX mliIndex = 0; mliIndex < this->GetCount(); mliIndex++ )
      {
         if( objLabel == MList< Pair< Label, Value > >::operator[]( mliIndex )->GetLabelRef() )
         {
            MASSERT( FALSE );

            throw( DictionaryException( DictionaryException::DuplicateLabel ) );
         }
      }

      MList< Pair< Label, Value > >::template AddSorted< Comparator >( objComparator, new Pair< Label, Value >( objLabel, objValue ) );

      return( *this );
   }


   Dictionary< Label, Value >& Replace( const Label& objLabel, const Value& objValue )
   {
      for( MLISTINDEX mliIndex = 0; mliIndex < this->GetCount(); mliIndex++ )
      {
         if( objLabel == MList< Pair< Label, Value > >::operator[]( mliIndex )->GetLabelRef() )
         {
            MList< Pair< Label, Value > >::operator[]( mliIndex )->SetValue( objValue );

            return( *this );
         }
      }

      MASSERT( FALSE );

      throw( DictionaryException( DictionaryException::LabelNotFound ) );
   }


   Dictionary< Label, Value >& Remove( const Label& objLabel )
   {
      for( MLISTINDEX mliIndex = 0; mliIndex < this->GetCount(); mliIndex++ )
      {
         if( objLabel == MList< Pair< Label, Value > >::operator[]( mliIndex )->GetLabelRef() )
         {
            delete this->RemoveMember( mliIndex );
            
            return( *this );
         }
      }

      MASSERT( FALSE );

      throw( DictionaryException( DictionaryException::LabelNotFound ) );
   }


   template< class Comparator >
   Dictionary< Label, Value >& Remove( const Comparator& comparator )
   {
      for( MLISTINDEX mliIndex = 0; mliIndex < this->GetCount(); mliIndex++ )
      {
         if( comparator.IsThisIt( MList< Pair< Label, Value > >::operator[]( mliIndex )->GetLabelRef() ) )
         {
            delete this->RemoveMember( mliIndex );
      
            return( *this );
         }
      }

      MASSERT( FALSE );

      throw( DictionaryException( DictionaryException::LabelNotFound ) );
   }


   Pair< Label, Value >* RemoveHead()
   {
      return( MList< Pair< Label, Value > >::RemoveHead() );
   }


   Pair< Label, Value >* RemoveTail()
   {
      return( MList< Pair< Label, Value > >::RemoveTail() );
   }


   Label& GetLabel( const Value& objValue )
   {
      this->Accelerate();

      for( MLISTINDEX mliIndex = 0; mliIndex < GetCount(); mliIndex++ )
      {
         if( objValue == MList< Pair< Label, Value > >::operator[]( mliIndex )->GetValueRef() )
         {
            return( MList< Pair< Label, Value > >::operator[]( mliIndex )->GetLabelRef() );
         }
      }

      MASSERT( FALSE );

      throw( DictionaryException( DictionaryException::ValueNotFound ) );
   }


   const Label& GetLabel( const Value& objValue ) const
   {
      this->Accelerate();

      for( MLISTINDEX mliIndex = 0; mliIndex < GetCount(); mliIndex++ )
      {
         if( objValue == MList< Pair< Label, Value > >::operator[]( mliIndex )->GetValueRef() )
         {
            return( MList< Pair< Label, Value > >::operator[]( mliIndex )->GetLabelRef() );
         }
      }

      MASSERT( FALSE );

      throw( DictionaryException( DictionaryException::ValueNotFound ) );
   }


   template< class Comparator, class ValueSubType >
   Label& GetLabelFromSubValue( const ValueSubType& objValue )
   {
      this->Accelerate();

      const Comparator comp( objValue );

      for( MLISTINDEX mliIndex = 0; mliIndex < GetCount(); mliIndex++ )
      {
         if( comp.IsEqual( MList< Pair< Label, Value > >::operator[]( mliIndex )->GetValueRef() ) )
         {
            return( MList< Pair< Label, Value > >::operator[]( mliIndex )->GetLabelRef() );
         }
      }

      MASSERT( FALSE );

      throw( DictionaryException( DictionaryException::ValueNotFound ) );
   }


   template< class Comparator, class ValueSubType >
   bool HasLabelFromSubValue( const ValueSubType& objValue )
   {
      this->Accelerate();

      const Comparator comp( objValue );

      for( MLISTINDEX mliIndex = 0; mliIndex < GetCount(); mliIndex++ )
      {
         if( comp.IsEqual( MList< Pair< Label, Value > >::operator[]( mliIndex )->GetValueRef() ) )
         {
            return( true );
         }
      }

      return( false );
   }


#ifdef _WIN32
   template< class Comparator, class ValueSubType >
   const Label& GetLabelFromSubValue( const ValueSubType& objValue ) const
   {
      Accelerate();

      const Comparator< ValueSubType > comp( objValue );

      for( MLISTINDEX mliIndex = 0; mliIndex < GetCount(); mliIndex++ )
      {
         if( comp.IsEqual( MList< Pair< Label, Value > >::operator[]( mliIndex )->GetValueRef() ) )
         {
            return( MList< Pair< Label, Value > >::operator[]( mliIndex )->GetLabelRef() );
         }
      }

      MASSERT( FALSE );

      throw( DictionaryException( DictionaryException::ValueNotFound ) );
   }
#endif


   Value& GetValue( const Label& objLabel )
   {
      this->Accelerate();

      for( MLISTINDEX mliIndex = 0; mliIndex < GetCount(); mliIndex++ )
      {
         if( objLabel == MList< Pair< Label, Value > >::operator[]( mliIndex )->GetLabelRef() )
         {
            return( MList< Pair< Label, Value > >::operator[]( mliIndex )->GetValueRef() );
         }
      }

      MASSERT( FALSE );

      throw( DictionaryException( DictionaryException::LabelNotFound ) );
   }


   MLISTINDEX GetCount() const throw()
   {
      return( MList< Pair< Label, Value > >::GetCount() );
   }


   MList< Label > GetLabels() const throw()
   {
      MList< Label > listLabels;

      for( MLISTINDEX mliIndex = 0; mliIndex < GetCount(); mliIndex++ )
      {
         listLabels.AddTail( new Label( MList< Pair< Label, Value > >::operator[]( mliIndex )->GetLabelRef() ) );
      }

      return( listLabels );
   }


   MList< Value > GetValues() const throw()
   {
      MList< Value > listValues;

      for( MLISTINDEX mliIndex = 0; mliIndex < GetCount(); mliIndex++ )
      {
         listValues.AddTail( new Value( MList< Pair< Label, Value > >::operator[]( mliIndex )->GetValueRef() ) );
      }

      return( listValues );
   }


   Pair< Label, Value >& GetByIndex( const MLISTINDEX mliIndex )
   {
      this->Accelerate();

      return( *MList< Pair< Label, Value > >::operator[]( mliIndex ) );
   }


   const Pair< Label, Value >& GetByIndex( const MLISTINDEX mliIndex ) const
   {
      this->Accelerate();

      return( *MList< Pair< Label, Value > >::operator[]( mliIndex ) );
   }


   Value& operator[]( const Label& objLabel )
   {
      this->Accelerate();

      for( MLISTINDEX mliIndex = 0; mliIndex < GetCount(); mliIndex++ )
      {
         if( objLabel == MList< Pair< Label, Value > >::operator[]( mliIndex )->GetLabelRef() )
         {
            return( MList< Pair< Label, Value > >::operator[]( mliIndex )->GetValueRef() );
         }
      }

      MASSERT( FALSE );

      throw( DictionaryException( DictionaryException::LabelNotFound ) );
   }


   const Value& GetValue( const Label& objLabel ) const
   {
      this->Accelerate();

      for( MLISTINDEX mliIndex = 0; mliIndex < GetCount(); mliIndex++ )
      {
         if( objLabel == MList< Pair< Label, Value > >::operator[]( mliIndex )->GetLabelRef() )
         {
            return( MList< Pair< Label, Value > >::operator[]( mliIndex )->GetValueRef() );
         }
      }

      MASSERT( FALSE );

      throw( DictionaryException( DictionaryException::LabelNotFound ) );
   }


   template< class Comparator >
   Value& GetValue( const Comparator& comparator ) const
   {
      this->Accelerate();

      for( MLISTINDEX mliIndex = 0; mliIndex < GetCount(); mliIndex++ )
      {
         if( comparator.IsThisIt( MList< Pair< Label, Value > >::operator[]( mliIndex )->GetLabelRef() ) )
         {
            return( MList< Pair< Label, Value > >::operator[]( mliIndex )->GetValueRef() );
         }
      }

      MASSERT( FALSE );

      throw( DictionaryException( DictionaryException::LabelNotFound ) );
   }


   const Value& operator[]( const Label& objLabel ) const
   {
      this->Accelerate();

      for( MLISTINDEX mliIndex = 0; mliIndex < GetCount(); mliIndex++ )
      {
         if( objLabel == MList< Pair< Label, Value > >::operator[]( mliIndex )->GetLabelRef() )
         {
            return( MList< Pair< Label, Value > >::operator[]( mliIndex )->GetValueRef() );
         }
      }

      MASSERT( FALSE );

      throw( DictionaryException( DictionaryException::LabelNotFound ) );
   }


#ifdef _WIN32
   template< class Comparator, class LabelSubType >
   Value& GetValueFromSubLabel( const LabelSubType& objLabel )
   {
      Accelerate();

      const Comparator< LabelSubType > comp( objLabel );

      for( MLISTINDEX mliIndex = 0; mliIndex < GetCount(); mliIndex++ )
      {
         if( comp.IsEqual( MList< Pair< Label, Value > >::operator[]( mliIndex )->GetLabelRef() ) )
         {
            return( MList< Pair< Label, Value > >::operator[]( mliIndex )->GetValueRef() );
         }
      }

      MASSERT( FALSE );

      throw( DictionaryException( DictionaryException::LabelNotFound ) );
   }


   template< class Comparator, class LabelSubType >
   const Value& GetValueFromSubLabel( const LabelSubType& objLabel ) const
   {
      Accelerate();

      const Comparator< LabelSubType > comp( objLabel );

      for( MLISTINDEX mliIndex = 0; mliIndex < GetCount(); mliIndex++ )
      {
         if( comp.IsEqual( MList< Pair< Label, Value > >::operator[]( mliIndex )->GetLabelRef() ) )
         {
            return( MList< Pair< Label, Value > >::operator[]( mliIndex )->GetValueRef() );
         }
      }

      MASSERT( FALSE );

      throw( DictionaryException( DictionaryException::LabelNotFound ) );
   }
#endif


   bool HasLabel( const Label& objLabel ) const
   {
      for( MLISTINDEX mliIndex = 0; mliIndex < this->GetCount(); mliIndex++ )
      {
         if( objLabel == MList< Pair< Label, Value > >::operator[]( mliIndex )->GetLabelRef() )
         {
            return( true );
         }
      }

      return( false );
   }


   template< class Comparator >
   bool HasLabel( const Comparator& comparator ) const
   {
      for( MLISTINDEX mliIndex = 0; mliIndex < this->GetCount(); mliIndex++ )
      {
         if( comparator.IsThisIt( MList< Pair< Label, Value > >::operator[]( mliIndex )->GetLabelRef() ) )
         {
            return( true );
         }
      }

      return( false );
   }


   bool HasValue( const Value& objValue ) const
   {
      for( MLISTINDEX mliIndex = 0; mliIndex < this->GetCount(); mliIndex++ )
      {
         if( objValue == MList< Pair< Label, Value > >::operator[]( mliIndex )->GetValueRef() )
         {
            return( true );
         }
      }

      return( false );
   }


   template< class Comparator >
   bool HasValue( const Comparator& comparator ) const
   {
      for( MLISTINDEX mliIndex = 0; mliIndex < this->GetCount(); mliIndex++ )
      {
         if( comparator.IsThisIt( MList< Pair< Label, Value > >::operator[]( mliIndex )->GetValueRef() ) )
         {
            return( true );
         }
      }

      return( false );
   }

   
   template< class Comparator >
   MLISTINDEX GetIndex( const Comparator& comparator ) const
   {
      for( MLISTINDEX mliIndex = 0; mliIndex < GetCount(); mliIndex++ )
      {
         if( comparator.IsThisIt( *MList< Pair< Label, Value > >::operator[]( mliIndex ) ) )
         {
            return( mliIndex );
         }
      }

      MASSERT( FALSE );

      throw( DictionaryException( DictionaryException::LabelNotFound ) );
   }


   virtual Dictionary< Label, Value >& Empty()
   {
      MList< Pair< Label, Value > >::Empty();

      return( *this );
   }


   // This method is to help when the typeMembers is mere than one level of indirection to the TYPE.
   // For example if MList< int** > the bool SortList( parameters ) will compare *(int**), i.e. int*.
   // This may be useful in some cases but in other cases I want to compare the int not their locations.
   template< class tComparator > void Sort( const tComparator& tcComparator ) throw()
   {
      MList< Pair< Label, Value > >::SortList( tcComparator );
   }


   template< class tExplorer > bool Find_H2T( MLISTINDEX* pmliFoundZBIndex, const tExplorer &teExplorer ) const
   {
	   return( MList< Pair< Label, Value > >::Find_H2T( pmliFoundZBIndex, teExplorer ) );
   }
};
