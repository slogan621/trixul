/*

Trixul Cross-Platform GUI Toolkit

Copyright (c) 2006, Syd Logan
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following disclaimer in the documentation 
and/or other materials provided with the distribution.
* Neither the name trixul nor the names of its contributors may be used to
endorse or promote products derived from this software without specific prior
written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR 
TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF 
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include "boxlayout.h"
#include "layout.h"
#include "document.h"
#include "spacer.h"
#include "button.h"

#include <math.h>

BoxLayout::BoxLayout() : m_orient(BoxLayout::Unknown), m_inLayout(false),
    m_initialLayout(true)
{
}

int BoxLayout::GetSpacerCount()
{       
    int count = 0;

    list<Element *> children;
    children = dynamic_cast<Element *>(this)->GetChildren();
    list<Element *>::iterator iter;

    for (iter = children.begin(); iter != children.end(); ++iter) {
        Widget *wChild = dynamic_cast<Widget *>((*iter));
        ElementType childType = wChild->GetType();
        if (childType == TYPE_SPACER)
            count++;
    }
    return count;
}

int BoxLayout::GetSpacerDenom()
{       
    int denom = 0;

    list<Element *> children;
    children = dynamic_cast<Element *>(this)->GetChildren();
    list<Element *>::iterator iter;

    for (iter = children.begin(); iter != children.end(); ++iter) {
        Widget *wChild = dynamic_cast<Widget *>((*iter));
        ElementType childType = wChild->GetType();
        if (childType == TYPE_SPACER) {
            Spacer *spacer = dynamic_cast<Spacer *>(wChild);
            denom += spacer->GetWeight(); 
        }
    }
    return denom;
}

PRStatus 
BoxLayout::CalculateSizeRequirements()
{
    Widget *w = dynamic_cast<Widget *>(this);

    if (w) {
        ElementType type = w->GetType();
        if (type == TYPE_BOX || type == TYPE_WINDOW || type == TYPE_SCROLLEDWINDOW || type == TYPE_SCROLLEDVIEW) { 
            int wTotal = 0, hTotal = 0;
            int wMax = -1, hMax = -1;
            list<Element *> children;
            Element *node = dynamic_cast<Element *>(this);
            children = node->GetChildren();
            BoxLayout::BoxOrientation orient; 
            GetOrientation(orient);
            bool isHorizontal = (type != TYPE_WINDOW && type != TYPE_SCROLLEDWINDOW && type != TYPE_SCROLLEDVIEW && (orient==BoxLayout::Horizontal && type == TYPE_BOX ? true : false));
            list<Element *>::iterator iter;
            for (iter = children.begin(); iter != children.end(); ++iter) {
                BoxLayout *boxChild = dynamic_cast<BoxLayout *>((*iter));
                if (boxChild) {
                    Widget *wChild = dynamic_cast<Widget *>(boxChild);
                    if (wChild) {
                        ElementType childType = wChild->GetType();
                        if (childType == TYPE_BOX || childType == TYPE_WINDOW || childType == TYPE_SCROLLEDWINDOW || childType == TYPE_SCROLLEDVIEW)
                            boxChild->CalculateSizeRequirements();
                        if (childType != TYPE_SPACER) {
                            int x, y, width, height;

                            wChild->GetGeometryRequest(x, y, width, height);
                            if (isHorizontal) {
                                wTotal += width;
                                if (height > hMax)
                                    hMax = height;
                            }
                            else {
                                hTotal += height;
                                if (width > wMax)
                                    wMax = width;
                            }
                        }
                    }
                }
            }
            if (type != TYPE_SPACER) {
                if (isHorizontal) 
                    w->SetGeometryRequest(0, 0, wTotal, hMax, GEOM_WIDTH | GEOM_HEIGHT); 
                else 
                    w->SetGeometryRequest(0, 0, wMax, hTotal, GEOM_WIDTH | GEOM_HEIGHT); 
            }
        }
    }
    return PR_SUCCESS;
}

PRStatus 
BoxLayout::ComputeBoxLayout(const int &x, const int &y, const int &width,
    const int &height)
{
    Element *node = dynamic_cast<Element *>(this);
    ElementType type = node->GetType();
    if (m_inLayout == false) {
        m_inLayout = true;
        Widget *w = dynamic_cast<Widget *>(this);
        if (w && (type != TYPE_WINDOW || GetInitialLayout()))
            w->SetGeometry(x, y, width, height, GEOM_X | GEOM_Y | GEOM_WIDTH | GEOM_HEIGHT);
        CalculateSizeRequirements();
        AdjustSpacers(width, height);
        ComputePositions(0, 0);
        m_inLayout = false;
    }
    SetInitialLayout(false);
    return PR_SUCCESS;
}

// distribute remaining space around widgets to adjacent spacers

PRStatus
BoxLayout::AdjustSpacers(const int widthRequest, const int heightRequest)
{
    Widget *w = dynamic_cast<Widget *>(this);
    if (!w)
        return PR_FAILURE;

    BoxLayout::BoxOrientation orient;
    GetOrientation(orient);
    ElementType type = w->GetType();
    bool isHorizontal = (type != TYPE_WINDOW && type != TYPE_SCROLLEDWINDOW && type != TYPE_SCROLLEDVIEW && (orient==BoxLayout::Horizontal && type == TYPE_BOX ? true : false));

    int spacerCount = GetSpacerCount();
    int spacerDenom = GetSpacerDenom();

    int xComputed, yComputed, widthComputed, heightComputed;
    int widthDelta, heightDelta;

    w->GetComputedGeometry(xComputed, yComputed, widthComputed, heightComputed);

    int spacerHeight = 0, spacerWidth = 0;

    if (isHorizontal && spacerCount) {
        spacerHeight = 0;
        widthDelta = widthRequest - widthComputed;
        spacerWidth = widthDelta;
    } else if (!isHorizontal && spacerCount) {
        spacerWidth = 0;
        heightDelta = heightRequest - heightComputed;
        spacerHeight = heightDelta;
    }

    list<Element *>::iterator iter; 
    list<Element *> children;    
    children = dynamic_cast<Element *>(this)->GetChildren();

    for (iter = children.begin(); iter != children.end(); ++iter) {
        Widget *wChild = dynamic_cast<Widget *>((*iter));
        ElementType type = wChild->GetType();

        if (type == TYPE_SCROLLEDVIEW) {
            int x, y, width, height;
            wChild->GetGeometryRequest(x, y, width, height);
            wChild->AdjustSpacers(width, height);
        } else if (type == TYPE_BOX || type == TYPE_WINDOW || type == TYPE_SCROLLEDWINDOW) {
            int x, y, width, height;
            wChild->GetGeometryRequest(x, y, width, height);
            if (children.size() == 1)
                wChild->AdjustSpacers(widthRequest, heightRequest);
            else if (!isHorizontal)
                wChild->AdjustSpacers(widthRequest, height);
            else
                wChild->AdjustSpacers(width, heightRequest);
        } else if (type == TYPE_SPACER) {
            Spacer *spacer = dynamic_cast<Spacer *>(wChild);
            int widthActual = spacerWidth, heightActual = spacerHeight;
            int weight = spacer->GetWeight();

            if (isHorizontal)
                widthActual = (int)(widthActual * ((double)weight / spacerDenom));
            else
                heightActual = (int)(heightActual * ((double)weight / spacerDenom));
            wChild->SetGeometryRequest(0, 0, widthActual, heightActual,
                GEOM_WIDTH | GEOM_HEIGHT);
        }
    }
    return PR_SUCCESS;
}

PRStatus
BoxLayout::ComputePositions(int x, int y)
{
    int xSave = x, ySave = y;
    Element *node = dynamic_cast<Element *>(this);

    if (!node)
        return PR_FAILURE;

    BoxLayout::BoxOrientation orient;
    GetOrientation(orient);

    ElementType type = node->GetType();

    if (type == TYPE_WINDOW) {
        x = 0;
        y = 0;
    }

    list<Element *>::iterator iter;
    list<Element *> children;
    children = dynamic_cast<Element *>(this)->GetChildren();
        
    // if there are children, lay them out

    for (iter = children.begin(); iter != children.end(); ++iter) {
        Widget *w = dynamic_cast<Widget *>((*iter));
        if (w) {
            ElementType childType = w->GetType();
            if (childType == TYPE_SCROLLEDVIEW)
                w->ComputePositions(0, 0);
            else {
                w->ComputePositions(x, y);
                int childX, childY, childWidth, childHeight;
                w->GetGeometryRequest(childX, childY, childWidth, childHeight);
                if ((type == TYPE_BOX && orient == BoxLayout::Vertical) || type == TYPE_WINDOW || type == TYPE_SCROLLEDVIEW || type == TYPE_SCROLLEDWINDOW)
                    y += childHeight;
                else if (type == TYPE_BOX && orient == BoxLayout::Horizontal)
                    x += childWidth;
            }
        }
    }

    Widget *w = dynamic_cast<Widget *>(this);
    if (w) {
        int tempX, tempY, curWidth, curHeight, widthRequest, heightRequest;

        w->GetGeometryRequest(tempX, tempY, widthRequest, heightRequest);
        w->GetGeometry(tempX, tempY, curWidth, curHeight);

#if defined(NOT_YET) 

        /* 

        XXX This code will cause the following XUL to layout the text widget
        inset by 50 columns. 

        <window name="main" title="&hello.title;" main="true" width="320" height="200" x="100" y="100" position="center, mouse">
            <script type="text/javascript" src="resources/content/simple.js"/>
            <box orient="vertical">
                <text editable="true" selectable="true" string="&text1.value;" width="100" height="40"/>
            </box>
        </window>

        Not quite sure what problem it *does* fix, so just commenting out 
        for the moment, to be fully removed later

        */

        ElementType parentType;
       
        if (type != TYPE_WINDOW)
            parentType = w->GetParent()->GetType();
        if (type != TYPE_BOX && type != TYPE_SPACER && type != TYPE_WINDOW && type != TYPE_SCROLLEDVIEW && type != TYPE_SCROLLEDWINDOW && parentType != TYPE_WINDOW) {
            int deltaX, deltaY;
            if (orient == BoxLayout::Vertical && widthRequest > curWidth) {
                deltaX = widthRequest - curWidth;
                xSave += deltaX >> 1;
            }
            if (orient == BoxLayout::Horizontal && heightRequest > curHeight) {
                deltaY = heightRequest - curHeight;
                ySave += deltaY >> 1;
            }
        }
#endif
        if (type != TYPE_WINDOW)   
            w->SetGeometry(xSave, ySave, widthRequest, heightRequest, 
                GEOM_X | GEOM_Y | GEOM_WIDTH | GEOM_HEIGHT);
    }
    return PR_SUCCESS;
}

PRStatus BoxLayout::SetOrientation(const BoxOrientation orient)
{
    m_orient = orient;
    return PR_SUCCESS;
}

PRStatus BoxLayout::GetOrientation(BoxOrientation &orient)
{
    if (m_orient == Unknown) {
        AnAttribute *attribute;
        static string val("");

        attribute = dynamic_cast<Element *>(this)->GetAttributeByName(string("orientation"));
        if (attribute)
            val = attribute->GetValue();
        else {
            attribute = dynamic_cast<Element *>(this)->GetAttributeByName(string("orient"));
            if (attribute)
                val = attribute->GetValue();
        }
        if (val == "horizontal")
            m_orient = Horizontal;
        else if (val == "vertical")
            m_orient = Vertical; 
    }

    orient = m_orient;
    return PR_SUCCESS;
}

Widget *
BoxLayout::GetLeftWidget(Element *element, ElementType type, Widget *node)
{
    Widget *ret = NULL;

    if (node && element) {
        list <Element *> children;
        list <Element *>::iterator iter;

        children = element->GetChildren();
        for (iter = children.begin(); iter != children.end(); ++iter) {
            Widget *temp = dynamic_cast<Widget *>(*iter);
            if (temp == node)
                break;
            else if (temp->GetType() == type)
                ret = temp;
        } 
    }
    return ret;
}

Widget *
BoxLayout::GetRightWidget(Element *element, ElementType type, Widget *node)
{
    Widget *ret = NULL;

    if (node && element) {
        list <Element *> children;
        list <Element *>::iterator iter;
        bool look = false;

        children = element->GetChildren();
        for (iter = children.begin(); iter != children.end(); ++iter) {
            Widget *temp = dynamic_cast<Widget *>(*iter);
            if (temp == node)
                look = true;
            else if (look && temp->GetType() == type) {
                ret = temp;
                break;
            }
        } 
    }
    return ret;
}

Widget *
BoxLayout::GetTopWidget(Element *element, ElementType type, Widget *node)
{
    Widget *ret = GetLeftWidget(element, type, node);
    return ret;
}

Widget *
BoxLayout::GetBottomWidget(Element *element, ElementType type, Widget *node)
{
    Widget *ret = GetRightWidget(element, type, node);
    return ret;
}

