/*
* Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: 
* RSisRegistrySession - external client registry session interface
* The defined fuctionality is used by clients to access the registry as a whole
* and to perform registry global searches.
* The access to a specific functionality is depends on the client capabilities
* and may be restricted.
*
*/


/**
 @file
 @publishedPartner
 @released
*/

#ifndef __SISREGISTRYSESSION_H__
#define __SISREGISTRYSESSION_H__

#include <e32std.h>
#include <f32file.h>
#include <swi/sisregistrylog.h>

#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK	
#include <usif/usifcommon.h>
#endif

// Use Char 0xFF to indicate no drive is selected
const TInt KNoDriveSelected = 0xFF;

namespace Swi
{
class CHashContainer;
class CSisRegistryPackage;

/**
 * External client registry session interface
 *
 * @publishedPartner
 * @released
 */
class RSisRegistrySession : public RSessionBase
	{
public:

	/**
	 * Connect to the server, attempt to start it if it is not yet running
	 *
	 * @return KErrNone if successful or an error code
	 */
	IMPORT_C TInt Connect();

	/**
	 * Returns an array of installed uids (excludes augmentations).
	 * The array is supplied by the client which is then populated.
	 * There should be no assumption about the order in which the uids are put
	 * in it as it is implementation dependent.
	 *
	 * @param aUids 	The array object to be populated.
	 * @capability      ReadUserData
	 *
	 */
	IMPORT_C void InstalledUidsL(RArray<TUid>& aUids);

	/**
	 * Returns an array of the curently installed packages (including augmentations).
	 *
	 *
 	 * @param aPackages The array of installed packages
	 * @capability      ReadUserData
	 *
	 */
	IMPORT_C void InstalledPackagesL(RPointerArray<CSisRegistryPackage>& aPackages);

	/**
	 * Checks whether or not the package identified by this UID is installed.
	 *
	 * @param aUid The uid to search for
	 *
	 * @return ETrue if the Uid is installed
	 *         EFalse otherwise
	 */
	IMPORT_C TBool IsInstalledL(TUid aUid);

	/**
	 * Checks whether or not the package, identified by this controller is installed.
	 *
	 * @param aController The controller (as raw binary representation) to search for
	 *
	 * @return ETrue if the controller is installed
	 *         EFalse otherwise
	 */
	IMPORT_C TBool IsInstalledL(TDesC8& aController);

	/**
	 * Query whether or not a file is tagged as modifiable.
	 * @note The hash value for non-modifiable content should stay the same.
	 *
	 * @param aFileName The filename (incl full path) to be used to search the entries
	 *
	 * @return ETrue if it can be modified
	 *         EFalse otherwise
	 */
	IMPORT_C TBool ModifiableL(const TDesC& aFileName);

	/**
	 * Returns a pointer to the hash for this file
	 *
	 * @param aFileName The filename (incl full path) to be searched for
	 *
	 * @return A new CHashContainer object representing the hash.
	 */
	IMPORT_C CHashContainer* HashL(const TDesC& aFileName);

	/**
	 * Returns a pointer to package containing this Sid
	 *
	 * @param aSid The Sid of an application for which the package is requested
	 *
	 * @return A new CSisRegistryPackage object associated with the Sid.
	 */
	IMPORT_C CSisRegistryPackage* SidToPackageL(TUid aSid);

	/**
	 * Queries the registry for an installed filename relating to a specific sid
	 *
	 * @param aSid the sid of a file that was installed
	 * @param aFileName on return the filename found
	 */
	IMPORT_C void SidToFileNameL(TUid aSid,TDes& aFileName);
	
	/**
	 * Queries the registry for an installed filename relating to a specific sid
	 * This function will return only a sid that is associated to a specific drive.
	 * This function leaves with KErrNotFound if not exact macth is found ( Sid associated to drive)
	 *
	 * @param aSid the sid of a file that was installed
	 * @param aFileName on return the filename found
	 * @param aDrive specifies on which drive the sid is expected to be found. 
	 */
	IMPORT_C void SidToFileNameL(TUid aSid,TDes& aFileName, TDriveUnit aDrive);

	IMPORT_C TBool IsSidPresentL(TUid aSid);

	/**
	 * Retrieve the enteries in log file
	 *
	 * @capability ReadUserData
	 * @param aLogFile The caller supplies an empty array of CLogEntry which is loaded
	 * with a enteries in log file. If Log file is not present then array count will be 
	 * zero.  The caller is responsible for deleting all memory.
	 */
	IMPORT_C void RetrieveLogFileL(RPointerArray<CLogEntry>& aLogFile);	

	/**
	 * Returns an array of the currently installed packages which can be removed
	 * (including augmentations).
	 *
	 * @capability      ReadUserData
	 * @param aPackages The array of installed packages which are removable
	 */
	IMPORT_C void RemovablePackagesL(RPointerArray<CSisRegistryPackage>& aPackages);


	/**
	 * Checks to see if a recovery needs to take place. If so, failed
	 * transactions are rolled back, and the cache regenerated.
	 */
	IMPORT_C void RecoverL();

#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK	
	/**
	@internalComponent
	 * Verifies whether a file is registered to any existing component. Please note that this function does not support wildcards - both in the file name, and in the list of owned files in the registry.
	 */
	IMPORT_C TBool IsFileRegisteredL(const TDesC& aFileName);
	
	/**
	@internalAll
	 * Returns the SCR component id for the base component (i.e. not an SP package/augmentation) with the given UID. 
	 */
	IMPORT_C Usif::TComponentId GetComponentIdForUidL(const TUid& aUid);	
#endif

protected:
	/**
     * @internalComponent
     */
	HBufC8* SendReceiveBufferLC(TInt aMessage);
	/**
     * @internalComponent
     */
	HBufC8* SendReceiveBufferLC(TInt aMessage, TPtrC8 aInputBuffer);
	/**
     * @internalComponent
     */
	HBufC8* SendReceiveBufferLC(TInt aMessage, const TDesC& aInputDescriptor);
	/**
	* @internalComponent
	*/
	HBufC8* SendReceiveBufferLC(TInt aMessage, TPtrC8 aInputBuffer, TInt aThirdArgument);
	};

} // namespace
#endif
