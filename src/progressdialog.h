/***************************************************************************
                        progressdialog.h  -  description
                             -------------------
    begin                : Sun Jul 1 2007
    copyright            : (C) 2007 by Dominik Seichter
    email                : domseichter@web.de
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef _PROGRESS_DIALOG_H_
#define _PROGRESS_DIALOG_H_

#include <QDialog>

#include "ui_progressdialog.h"

#include "krenamefile.h"

class BatchRenamer;

class ProgressDialog : public QDialog {
    Q_OBJECT

 public:
    ProgressDialog( ESplitMode eSplitMode, unsigned int dot, QWidget* parent = NULL );

    /** Set the destination of the files
     *  so that the user can easily open a file browser
     *  theres.
     *
     *  @param dest the destination directory
     */
    inline void setDestination( const KUrl & dest );

    /** Set the number of total steps in the progressbar
     *  which is displayed to the user.
     *
     *  @param t the number of steps
     */
    inline void setProgressTotalSteps( int t );
    
    /** Set the current progress
     *
     *  @param p current progress (must be smaller 
     *           than the total number of steps and bigger than 0)
     *
     *  \see setProgressTotalSteps
     */
    inline void setProgress( int p );

    /** 
     * @returns true if the user has cancelled the operation (otherwise false)
     */
    inline bool wasCancelled() const;

    /*
    inline void setDestination( const KURL & dir );
    inline void setRenamedFiles( RenamedList* list, unsigned int size ) ;
    inline void setCreatedDirectories( const KURL::List & list );
    *    */

    //void done( int errors, int successfull, bool allowundo );

    /** Print an information message to the user
     *
     *  @param text message
     *  @param pixmap an optional icon
     */
    void print( const QString & text, const QString & pixmap = NULL );

    /** Print an error message to the user
     *
     *  @param text message
     */
    void error( const QString & text );

    /** Print a warning message to the user
     *
     *  @param text message
     */
    void warning( const QString & text );

    /** Renaming is done.
     *
     *  Mostly used to disable the cancel button
     *  and enable other buttons
     *
     *  @param enableMore if ture the more button will be enabled
     *  @param enableUndo if true the undo button will be enabled
     *  @param batchRename is a handle to a batchrenamer instance that can be used to undo the operation and
     *                     to determine URLs for a new renaming session
     *  @param errros the number of errors that have occurred. If errors have occured the user
     *                has the extra possibility to only rename files with errors again
     */
    void renamingDone( bool enableMore, bool enableUndo, BatchRenamer* renamer, int errors ); 

 private slots:
     /** Called when the user cancels the operation
      */
     void slotCancelled();

     /** Called when the users presses the "Open Destination..." button
      */
     void slotOpenDestination();

     /** Called when the user wants to rename some more files
      */
     void slotRestartKRename();

     /** Called when the user wants to rename all files again
      *  that have been processed without an error.
      */
     void slotRenameProcessedAgain();

     /** Called when the user wants to rename all files again
      *  that have been processed with an error.
      */
     void slotRenameUnprocessedAgain();

     /** Called when the user wants to rename all files again.
      */
     void slotRenameAllAgain();

     /** Called when the user wants instant undo
      */
     void slotUndo();

 private:
    Ui::ProgressDialog m_widget;

    bool m_canceled;         ///< the current canceled state
    BatchRenamer* m_renamer; ///< A BatchRenamer that can undo the operation
    KUrl m_dest;             ///< files destination

    QPushButton* m_buttonUndo;
    QPushButton* m_buttonMore;
    QPushButton* m_buttonDest;
    QPushButton* m_buttonClose;

    QAction*     m_actProcessed;
    QAction*     m_actUnprocessed;

    ESplitMode   m_eSplitMode;
    unsigned int m_dot;
};

void ProgressDialog::setDestination( const KUrl & dest )
{
    m_dest = dest;
}

void ProgressDialog::setProgressTotalSteps( int t )
{
    m_widget.bar->setMaximum( t );
}

void ProgressDialog::setProgress( int p )
{
    m_widget.bar->setValue( p );
}

bool ProgressDialog::wasCancelled() const
{ 
    return m_canceled;
}

#endif // _PROGRESS_DIALOG_H_
