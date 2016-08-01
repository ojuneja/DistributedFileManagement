/* 
 * @(#)Modattrs.java	1.6 01/05/24
 * 
 * Copyright 1997, 1998, 1999 Sun Microsystems, Inc. All Rights
 * Reserved.
 * 
 * Sun grants you ("Licensee") a non-exclusive, royalty free,
 * license to use, modify and redistribute this software in source and
 * binary code form, provided that i) this copyright notice and license
 * appear on all copies of the software; and ii) Licensee does not 
 * utilize the software in a manner which is disparaging to Sun.
 *
 * This software is provided "AS IS," without a warranty of any
 * kind. ALL EXPRESS OR IMPLIED CONDITIONS, REPRESENTATIONS AND
 * WARRANTIES, INCLUDING ANY IMPLIED WARRANTY OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE OR NON-INFRINGEMENT, ARE 
 * HEREBY EXCLUDED.  SUN AND ITS LICENSORS SHALL NOT BE LIABLE 
 * FOR ANY DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, 
 * MODIFYING OR DISTRIBUTING THE SOFTWARE OR ITS DERIVATIVES. IN 
 * NO EVENT WILL SUN OR ITS LICENSORS BE LIABLE FOR ANY LOST 
 * REVENUE, PROFIT OR DATA, OR FOR DIRECT, INDIRECT, SPECIAL,
 * CONSEQUENTIAL, INCIDENTAL OR PUNITIVE DAMAGES, HOWEVER 
 * CAUSED AND REGARDLESS OF THE THEORY OF LIABILITY, ARISING OUT 
 * OF THE USE OF OR INABILITY TO USE SOFTWARE, EVEN IF SUN HAS 
 * BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * 
 * This software is not designed or intended for use in on-line
 * control of aircraft, air traffic, aircraft navigation or aircraft
 * communications; or in the design, construction, operation or
 * maintenance of any nuclear facility. Licensee represents and warrants
 * that it will not use or redistribute the Software for such purposes.  
 */

import javax.naming.*;
import javax.naming.directory.*;

import java.util.Hashtable;

/**
 * Demonstrates how to make modifications to an object's attributes.
 * It remove an attribute, adds an attribute value, and replaces
 * an attribute value.
 * After making the modifications, it replaces the object's attributes
 * with its original attributes.
 *
 * usage: java Modattrs
 */

class Modattrs {
    public static void main(String[] args) {

	// Set up the environment for creating the initial context
	Hashtable env = new Hashtable(11);
	env.put(Context.INITIAL_CONTEXT_FACTORY, 
	    "com.sun.jndi.ldap.LdapCtxFactory");
	env.put(Context.PROVIDER_URL, "ldap://localhost:389/o=JNDITutorial");

	try {
	    // Create the initial context
	    DirContext ctx = new InitialDirContext(env);
	    String name = "cn=Ted Geisel, ou=People";

	    // Save original attributes
	    Attributes orig = ctx.getAttributes(name, 
		new String[]{"mail", "telephonenumber", "jpegphoto"});

	    // Specify the changes to make
	    ModificationItem[] mods = new ModificationItem[3];

	    // Replace the "mail" attribute with a new value
	    mods[0] = new ModificationItem(DirContext.REPLACE_ATTRIBUTE,
		new BasicAttribute("mail", "geisel@wizards.com"));

	    // Add additional value to "telephonenumber"
	    mods[1] = new ModificationItem(DirContext.ADD_ATTRIBUTE,
		new BasicAttribute("telephonenumber", "+1 555 555 5555"));

	    // Remove the "jpegphoto" attribute
	    mods[2] = new ModificationItem(DirContext.REMOVE_ATTRIBUTE,
		new BasicAttribute("jpegphoto"));
	    
	    // Perform the requested modifications on the named object
	    ctx.modifyAttributes(name, mods);

	    // Check attributes
	    System.out.println("**** new attributes *****");
	    GetattrsAll.printAttrs(ctx.getAttributes(name));

	    // Revert changes
	    ctx.modifyAttributes(name, DirContext.REPLACE_ATTRIBUTE, orig);

	    // Check that the attributes got restored
	    System.out.println("**** reverted to original attributes *****");
	    GetattrsAll.printAttrs(ctx.getAttributes(name));

	    // Close the context when we're done
	    ctx.close();
	} catch (Exception e) {
	    e.printStackTrace();
	}
    }
}
