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

#include "cocoagridlistimpl.h"
#include "gridlistdatasource.h"

CocoaGridListImpl::CocoaGridListImpl()
{
	m_gridlist = nil;
    m_dataSource = nil;
}

CocoaGridListImpl::~CocoaGridListImpl()
{
    if (m_dataSource) 
        [m_dataSource release];
    if (m_gridlist)
        [m_gridlist release];
    if (m_scrolledWindow)
        [m_scrolledWindow release];
}

PRStatus CocoaGridListImpl::Create()
{
    NSRect graphicsRect = NSMakeRect(0.0, 0.0, 0.0, 0.0);

	// create the gridlist

	m_gridlist = [[NSTableView alloc] initWithFrame:graphicsRect];

	if (m_gridlist) {
        [m_gridlist setAllowsMultipleSelection: true];

        // setup the datasource

        [m_gridlist setHeaderView:nil];
        m_dataSource = [[GridListDataSource alloc] init];
        if (m_dataSource) {
            [m_gridlist setDataSource: m_dataSource];
            //[m_gridlist reloadData];
        }

        m_view = (NSView *) m_gridlist;

        NSString *id = [NSString stringWithCString: "data"];
        NSTableColumn *column = [[NSTableColumn alloc] initWithIdentifier: id];

        if (column) {
            [m_gridlist addTableColumn: column];
            //[m_gridlist sizeToFit];
        }

        m_scrolledWindow = [[NSScrollView alloc] init];

        if (m_scrolledWindow) {
	        m_view = (NSView *) m_scrolledWindow;
            [m_scrolledWindow setBorderType:NSNoBorder];
            [m_scrolledWindow setHasVerticalScroller:YES];
            [m_scrolledWindow setHasHorizontalScroller:NO];

            [m_scrolledWindow setAutoresizingMask:NSViewWidthSizable |
                NSViewHeightSizable];

            // add the tableview as a child of the scrollview 

            [m_scrolledWindow setDocumentView: m_gridlist];

            // now parent the scrollview widget

		    WidgetImpl *parentImpl = GetParent();

		    if (parentImpl) {

			    NSView *parentView = 
                    dynamic_cast<CocoaWidgetImpl *>(parentImpl)->GetView();

			    // add the scrolledWindow to the parent view

			    if (parentView) {
				    [parentView addSubview: m_view];
				    return PR_SUCCESS;
                }
			}
		}
	}
	return PR_FAILURE;
}

PRStatus 
CocoaGridListImpl::GetGeometry(int &x, int &y, int &width, int &height)
{
    if (m_gridlist) {
        NSRect graphicsRect;

        graphicsRect = [m_gridlist frame];
        x = static_cast<int>(graphicsRect.origin.x);
        y = static_cast<int>(graphicsRect.origin.y);
        width = static_cast<int>(graphicsRect.size.width);
        height = static_cast<int>(graphicsRect.size.height);

        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus 
CocoaGridListImpl::SetGeometry(const int &x, const int &y, 
    const int &width, const int &height, const char &mask)
{    
    int newY = AdjustY(y, height);

    if (m_gridlist && m_scrolledWindow) {
        NSRect graphicsRect;
        if (mask == GEOM_ALL)
            graphicsRect = NSMakeRect(x, newY, width, height);
        else {
            graphicsRect = [m_gridlist frame];
            if (mask & GEOM_X)
                graphicsRect.origin.x = x;
            if (mask & GEOM_Y)
                graphicsRect.origin.y = newY;
            if (mask & GEOM_WIDTH)
                graphicsRect.size.width = width;
            if (mask & GEOM_HEIGHT)
                graphicsRect.size.height = height;
        }

        [m_scrolledWindow setFrame: graphicsRect];
        return PR_SUCCESS;
    }
    return PR_FAILURE;
}

PRStatus CocoaGridListImpl::Disable()
{
    NSView *sView = (NSView *) m_gridlist;
    if (m_view) {
        NSControl *control = (NSControl *) m_view;
        [control setEnabled:NO];
        if (sView) {
            NSControl *control = (NSControl *) sView;
            [control setEnabled:NO];
            return PR_SUCCESS;
        }
    }
    return PR_FAILURE;
}

PRStatus CocoaGridListImpl::Enable()
{
    NSView *sView = (NSView *) m_gridlist;
    if (m_view) {
        NSControl *control = (NSControl *) m_view;
        [control setEnabled:YES];
        if (sView) {
            NSControl *control = (NSControl *) sView;
            [control setEnabled:YES];
            return PR_SUCCESS;
        }
    }
    return PR_FAILURE;
}

PRStatus CocoaGridListImpl::Show()
{
    // show the widget

    NSView *sView = (NSView *) m_gridlist;
    if (m_view) {
        [m_view setHidden: NO];
        [m_view display];
        if (sView) {
            [sView setHidden: NO];
            [sView display];
            return PR_SUCCESS;
        }
    }
    return PR_FAILURE;
}

PRStatus CocoaGridListImpl::Hide()
{
    // show the widget

    NSView *sView = (NSView *) m_gridlist;
    if (m_view) {
        [m_view setHidden: YES];
        if (sView) {
            [sView setHidden: YES];
            return PR_SUCCESS;
        }
    }
    return PR_FAILURE;
}

PRStatus CocoaGridListImpl::GetSelectionCount(int &count)
{
    PRStatus ret = PR_FAILURE;
    if (m_gridlist) {
        count = [m_gridlist numberOfSelectedRows];
        ret = PR_SUCCESS;
    }
    return ret;
}

PRStatus CocoaGridListImpl::GetSelection(const int index, string &value)
{
printf("CocoaGridListImpl::GetSelection %d\n", index);
    PRStatus ret = PR_FAILURE;
    if (m_gridlist) {
        NSIndexSet *indexSet = [m_gridlist selectedRowIndexes];
        if (indexSet && index < (int) [indexSet count] ) {
            int count = 0;
            unsigned int curIndex = [indexSet firstIndex];
            while (curIndex != NSNotFound && count < index) {
                curIndex = [indexSet indexGreaterThanIndex: curIndex];
                count++;
            }
                 
            if (curIndex != NSNotFound) {
                NSString *theValue = (NSString *) [m_dataSource getItemAtIndex:curIndex];
                if (theValue) {
                    value = [theValue cStringUsingEncoding: NSUTF8StringEncoding];
printf("CocoaGridListImpl::GetSelection: found value %s\n", value.c_str());
                    ret = PR_SUCCESS;
                } 
            }
        }
    }
    return ret;
}

PRStatus CocoaGridListImpl::RemoveItemByValue(const string &value)
{
printf("Entering CocoaGridListImpl::RemoveItemByValue\n");
    PRStatus ret = PR_FAILURE;
    NSString *str = [NSString stringWithCString: value.c_str()];
    if (str && m_gridlist && m_dataSource) {
        [m_dataSource removeItemByValue: str];
        [m_gridlist reloadData];
        [m_gridlist tile];
        int numRows = [m_gridlist numberOfRows];
        int numCols = [m_gridlist numberOfColumns];
        printf("Number of rows %d number of columns %d\n", 
            numRows, numCols);
        ret = PR_SUCCESS;
    }
    return ret;
}

PRStatus CocoaGridListImpl::RemoveItemByPosition(const int position)
{
printf("Entering CocoaGridListImpl::RemoveItemByPosition %d\n", position);
    PRStatus ret = PR_FAILURE;
    if (m_gridlist && m_dataSource) {
        int numRows = [m_gridlist numberOfRows];
        if (position <= numRows && position > 0) {
            [m_dataSource removeItem: position - 1]; 
            [m_gridlist reloadData];
            [m_gridlist tile];
            ret = PR_SUCCESS;
        }
    }
    return ret;
}

PRStatus CocoaGridListImpl::AddItem(const string &value, const int position)
{
printf("Entering CocoaGridListImpl::AddItem\n");
    PRStatus ret = PR_FAILURE;
    NSString *str = [NSString stringWithCString: value.c_str()];
    if (str && m_gridlist && m_dataSource) {
        int numRows = [m_gridlist numberOfRows];
        if (position <= numRows && position >= 0) {
            [m_dataSource addItem: str atIndex:position];
            [m_gridlist reloadData];
            [m_gridlist tile];
            int numRows = [m_gridlist numberOfRows];
            int numCols = [m_gridlist numberOfColumns];
            printf("Number of rows %d number of columns %d\n", 
                numRows, numCols);
            ret = PR_SUCCESS;
        }
    }
    return ret;
}

PRStatus CocoaGridListImpl::AppendItem(const string &value)
{
printf("Entering CocoaGridListImpl::AppendItem\n");
    PRStatus ret = PR_FAILURE;
    NSString *str = [NSString stringWithCString: value.c_str()];
    if (str && m_gridlist && m_dataSource) {
        [m_dataSource addObject: str];
        [m_gridlist reloadData];
        [m_gridlist tile];
        int numRows = [m_gridlist numberOfRows];
        int numCols = [m_gridlist numberOfColumns];
        printf("Number of rows %d number of columns %d\n", 
            numRows, numCols);
        ret = PR_SUCCESS;
    }
    return ret;
}

