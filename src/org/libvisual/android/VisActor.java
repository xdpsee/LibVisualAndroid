/* Libvisual - The audio visualisation framework.
 * 
 * Copyright (C) 2004-2006 Dennis Smit <ds@nerds-incorporated.org>
 * Copyright (C) 2012 Daniel Hiepler <daniel-lva@niftylight.de>         
 *
 * Authors: Dennis Smit <ds@nerds-incorporated.org>
 *          Daniel Hiepler <daniel-lva@niftylight.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

package org.libvisual.android;




/** VisActor wrapper */
public class VisActor
{
    public int VisActor;
    public VisPlugin plugin;
        
    /** implemented by visual.c */
    private native int actorNew(String name);
    private native int actorUnref(int actorPtr);
    private native int actorGetSupportedDepth(int actorPtr);
    private native int actorVideoNegotiate(int actorPtr, int rundepth, boolean noevent, boolean forced);
    private native int actorGetPlugin(int actorPtr);


    public VisActor(int actorPtr)
    {
        VisActor = actorPtr;    
        plugin = new VisPlugin(actorGetPlugin(VisActor));
    }
        
    public VisActor(String name)
    {
        VisActor = actorNew(name);
        plugin = new VisPlugin(actorGetPlugin(VisActor));
    }

    public int getSupportedDepth()
    {
        return actorGetSupportedDepth(VisActor);
    }

        
    public void videoNegotiate(int rundepth, boolean noevent, boolean forced)
    {
        actorVideoNegotiate(VisActor, rundepth, noevent, forced);
    }

        
    @Override
    public void finalize()
    {
        actorUnref(VisActor);
    }
}
