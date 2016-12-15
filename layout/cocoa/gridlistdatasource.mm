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

#import <Cocoa/Cocoa.h>
#import "gridlistdatasource.h"

@implementation GridListDataSource

- (id) init
{
    if (!m_dataSource) {
        m_dataSource = [[NSMutableArray alloc] init];
    }
    return [super init];
}

- (void) addObject:(NSString *) anItem 
{
    if (m_dataSource)
        [m_dataSource addObject: anItem];
}

- (void) addItem:(NSString *) anItem atIndex:(int) rowIndex
{
    if (m_dataSource)
        [m_dataSource insertObject: anItem atIndex: rowIndex];
}

- (id) getItemAtIndex:(int) rowIndex
{
    if (m_dataSource)
        return [m_dataSource objectAtIndex:rowIndex];
    return nil;
}

- (void) removeItem:(int) rowIndex
{
    if (m_dataSource)
        [m_dataSource removeObjectAtIndex: rowIndex];
}

- (void) removeItemByValue:(NSString *) value
{
    if (m_dataSource)
        [m_dataSource removeObject: value];
}

- (void) dealloc
{
    if (m_dataSource) {
        [m_dataSource release];
        [super dealloc];
        m_dataSource = nil;
    }
}

// just returns the item for the right row
- (id) tableView:(NSTableView *) aTableView 
objectValueForTableColumn:(NSTableColumn *) aTableColumn row:(int) rowIndex
{  
    printf("objectValueForTableColumn %d\n", rowIndex);
    if (m_dataSource) {
        printf("objectValueForTableColumn %d\n", rowIndex);
        return [m_dataSource objectAtIndex:rowIndex];  
    }
    return nil;
}

// just returns the number of items we have.
- (int) numberOfRowsInTableView:(NSTableView *)aTableView
{
    if (m_dataSource) {
        int nrows = [m_dataSource count];  
        printf("numberOfRowsInTableView: nrows = %d\n", nrows);
        return nrows;
    }
    return 0;
}

- (void) tableView:(NSTableView *) aTableView
setObjectValue: anObject forTableColumn:(NSTableColumn *) aTableColumn 
row:(int) rowIndex

{
    if (m_dataSource) {
        printf("setObjectValue\n");
        id theRecord;
        NSParameterAssert(rowIndex >= 0 && rowIndex < (int) [m_dataSource count]);
        theRecord = [m_dataSource objectAtIndex:rowIndex];
        [theRecord setObject:anObject forKey:[aTableColumn identifier]];
    }
    return;
}

@end
