﻿//-------------------------------------------------------------------------------------------------
// File : App.h
// Desc : Sample Application.
// Copyright(c) Project Asura. All right reserved.
//-------------------------------------------------------------------------------------------------

#ifndef __APP_H__
#define __APP_H__

//-------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------
#include <asdxApp.h>
#include "Dice.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
// App class
///////////////////////////////////////////////////////////////////////////////////////////////////
class App : public asdx::Application
{
    //=============================================================================================
    // list of friend classes and methods.
    //=============================================================================================
    /* NOTHING */

public:
    //=============================================================================================
    // public variables.
    //=============================================================================================
    /* NOTHING */

    //=============================================================================================
    // public methods.
    //=============================================================================================
    App();
    virtual ~App();

protected:
    //=============================================================================================
    // protected variables.
    //=============================================================================================
    /* NOTHING */

    //=============================================================================================
    // protected methods.
    //=============================================================================================
    bool OnInit         ()                                    override;
    void OnTerm         ()                                    override;
    void OnFrameMove    (asdx::FrameEventArgs& param)         override;
    void OnFrameRender  (asdx::FrameEventArgs& param)         override;
    void OnResize       (const asdx::ResizeEventArgs& param)  override;
    void OnKey          (const asdx::KeyEventArgs& param)     override;
    void OnMouse        (const asdx::MouseEventArgs& param)   override;

private:
    //=============================================================================================
    // private variables.
    //=============================================================================================
    Dice        m_Dice;     //!< サイコロ.
    u32         m_Count;

    //=============================================================================================
    // private methods.
    //=============================================================================================
    void SetDiceCount( u32 count );
};


#endif//__APP_H__