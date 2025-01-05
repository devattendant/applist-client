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
* SisRegistry - client registry entry(package) interface
* Clients use the defined interface to access and querry information
* specific to a single installed package.
* The access to specific functionality depends on the client capabilities
* and may be restricted.
*
*/


/**
 @file
 @released
 @publishedPartner
*/

#ifndef __SISREGISTRYENTRY_H__
#define __SISREGISTRYENTRY_H__

#include <e32std.h>
#include <swi/sispackagetrust.h>

class CX509Certificate;

namespace Swi
{
class RSisRegistrySession;
class CSisRegistryPackage;
class CSisRegistryDependency;
class CHashContainer;
class TSisTrustStatus;

/**
 * Client registry entry interface
 *
 * @publishedPartner
 * @released
 */
class RSisRegistryEntry : public RSubSessionBase
	{
public:

	/**
	 * Opens the base package entry by specifying a UID.
	 *
	 * - A base package is a self-sufficient package which does
	 * not depend on other packages with the same UID. It is also completely removed
	 *  together with all related augmentation packages.
	 *
	 * - An agmentation package shares a UID with a base package and 'augments' the base package.
	 * It can only be installed only when the related base package is installed
	 * It can be uninstalled separatelly (without removing base package)
	 *
	 *
	 * @param aSession	The open RSisRegistrySession to use
	 * @param aUid		The UID identifying the entry
	 * @return			KErrNone if successful, otherwise an error code
	 */
	IMPORT_C TInt Open(RSisRegistrySession &aSession, TUid aUid);

	/**
	 * Open a registry entry (includes augmentations) by specifying a package
	 *
	 * @param aSession The open RSisRegistrySession to use
	 * @param aPackage The package to open
	 *
	 * @return KErrNone if successful, or an error code
	 */
	IMPORT_C TInt OpenL(RSisRegistrySession &aSession, const CSisRegistryPackage& aPackage);

	/**
	 * Open a registry entry (includes augmentations) by specifying a package
	 * and vendor name.
	 *
	 * @param aSession		The open RSisRegistrySession to use
	 * @param aPackageName	The name of the package
	 * @param aVendorName	The name of the vendor
	 * @return				KErrNone if successful, otherwise an error code
	 */
	IMPORT_C TInt Open(RSisRegistrySession &aSession, const TDesC& aPackageName, const TDesC& aVendorName);

	/**
	 * Closes the registry entry by closing the sub-session
	 */
	IMPORT_C void Close();

	/**
	 * Indicates whether the package is currently on the device (i.e. not on
	 * removable media that is not inserted).
	 *
	 * @return	ETrue if the package is on the device;
	 *			EFalse otherwise
	 */
	IMPORT_C TBool IsPresentL();

	/**
	 * Indicates whether the package is signed
	 *
	 * @return	ETrue the package is signed;
	 *			EFalse otherwise
	 */
	IMPORT_C TBool IsSignedL();

	/**
	 * Indicates the level of trust associated with the package
	 *
	 * @return	The level of trust
	 * @deprecated Will be replaced in the future by a more complete TrustStatusL method.
	 */
	IMPORT_C TSisPackageTrust TrustL() const;

	/**
	 * The time at which the trust level was established
	 *
	 * @return The time at which the trust level was established
	 * @deprecated Will be replaced in the future by a more complete TrustStatusL method.
	 */
	IMPORT_C TTime TrustTimeStampL() const;


    /**
	 * The trust status object for the entry. This supercedes information
	 * provided by the deprecated TrustL and TrustTimeStampL methods.
	 *
	 * @return The trust status information associated with
	 * this entry.
	 */
	IMPORT_C TSisTrustStatus TrustStatusL();

	/**
	 * Indicates whether or not the package is installed on read-only media
	 *
	 * 
	 * @return	ETrue if any drive used by this package is read-only;
	 *			EFalse otherwise
	 */
#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK		 
	/**
	 * N.B. Using Usif::RSoftwareComponentRegistry::IsComponentOnReadOnlyDriveL 
	 * will return different result when we have an empty ROM stub or when all 
	 * the files of a ROM based package are eclipsed, since SCR only considers
	 * the current list of files registered with it.
	 */
#endif
	IMPORT_C TBool IsInRomL();



	/**
	 * Indicates whether or not the package augments another package
	 *
	 * @return	ETrue if the package is an augmentation;
	 *			EFalse otherwise
	 */
	IMPORT_C TBool IsAugmentationL();

	/**
	 * Gets the version of this package
	 *
	 * @return	The version
	 */
	IMPORT_C TVersion VersionL();

	/**
	 * Gets the installed language for this package
	 *
	 * @return	The language
	 */
	IMPORT_C TLanguage LanguageL();

	/**
	 * Gets the UID of this package
	 *
	 * @return	The UID
	 */
	IMPORT_C TUid UidL();

	/**
	 * Gets the name of a package
	 *
	 * @return	The name of the package
	 */
	IMPORT_C HBufC* PackageNameL();

	/**
	 * Gets the unique vendor name of this package
	 *
	 * @return	The unique vendor name
	 */
	IMPORT_C HBufC* UniqueVendorNameL();

	/**
	 * Gets the localised vendor name of a package
	 *
	 * @return	The localised name of the vendor
	 */
	IMPORT_C HBufC* LocalizedVendorNameL();

	/**
	 * Returns an array of Sids (executables). The array is supplied by the
	 * client which is then populated.
	 *
	 * @param aSids	 On return, the array object to be populated.
	 */
	IMPORT_C void SidsL(RArray<TUid>& aSids);

	/**
	 * Provides a list of files installed by this package. This function may also return
	 * a file specification that contains wildcard characters ('?' and/or '*') if the 
	 * package is a rom 'stub' sis file. Wildcard file specifications are NOT expanded and
	 * in this instance, the Count() member function on 'aFiles' cannot be used as an
	 * indicator of the number of files within the package.
	 *
	 * @param aFiles	The array of files to be populated.
	 */
	IMPORT_C void FilesL(RPointerArray<HBufC>& aFiles);

	/**
     *  Returns the size of the installation excluding the size of
     *  other embedded packages
     *
     *  @return  A TInt64 value of the total installation size
     */
	IMPORT_C TInt64 SizeL();

	/**
	 * Provides the certificate chains associated with this package
	 *
	 * @param aCertificateChains	The array of certificate chains
	 */
	IMPORT_C void CertificateChainsL(RPointerArray<HBufC8>& aCertificateChains);

	/**
	 * Gets the value of a property within a package
	 *
	 * @param aKey	The key to search for
	 * @return		The value of this key if found;
	                KErrNotFound otherwise.
	 */
	IMPORT_C TInt PropertyL(TInt aKey);

	/**
	 * Returns all augmentations to this package. If no augmentations exist,
	 * the lists are empty
	 *
	 * @param aPackageNames	On return, a list of PackageNames
	 * @param aVendorNames	On return, the corresponding list of VendorNames
	 *
	 */
	IMPORT_C void AugmentationsL(RPointerArray<HBufC>& aPackageNames, RPointerArray<HBufC>& aVendorNames);

    /**
	 * Returns all augmentations to this package. If no augmentations exist,
	 * the list is empty
	 *
	 * @param aPackages The array of CSisRegistryPackages to be populated
	 */
    IMPORT_C void AugmentationsL(RPointerArray<CSisRegistryPackage>& aPackages);
	
	/**
	 * Gets the number of augmentations to this package.
	 * @return the number of augmentations to this package
	 */
	IMPORT_C TInt AugmentationsNumberL();
	
    /**
     *  Returns the hash value of a selected file, identified by its name
     *
     *  @param aFileName the full file path
     *  @return A new CHashContainer object
     */
	IMPORT_C CHashContainer* HashL(const TDesC& aFileName);

	 /**
	 *  Returns the package of a current entry
	 *
	 *  @return A new CSisRegistryPackage object
	 */
	IMPORT_C CSisRegistryPackage* PackageL();

	/**
	 * Return the controllers associated with this package as raw binary data
	 *
	 * @param aControllers The array of controllers to be populated.
	 *
	 */
	IMPORT_C void ControllersL(RPointerArray<HBufC8>& aControllers);

	/**
	 Returns what drive the user selected for files in the Sis file that
	 did not specify drive.It returns KNoDriveSelected if user is not prompted for drive selection.
	 For ROM stub packages it returns zero instead of KNoDriveSelected
	 To determine the set of drives that files were installed to for this registry entry @see RSisRegistryEntry::InstalledDrivesL().
	 @return TChar The drive selected
	 @see KNoDriveSelected
	 */
	IMPORT_C TChar SelectedDriveL();

	/**
	Determines whether the base package or any of the partial upgrades
	require all applications within this package to be to be shutdown before
	uninstalling the package.

	@return Whether to shutdown all applications at un-install.
	*/
	IMPORT_C TBool ShutdownAllAppsL();

	/**
	The function is used to re-verify the signature and certificate of the
	SIS Controller associated to the RSisRegistryEntry object.

	aX509CertArray parameter is used to give set of trusted root certificates
	that are used for validation of the SIS Controller certificate. If the
	set given as parameter is empty, Symbian implementation can fetch the
	set of trusted root certificates from Certificate Management. If the set
	is not empty, then root certificates should not be fetched from
	Certificate Mgmt, but only the root certificates from the set given as
	parameter should be used for SIS Controller certificate validation.

	@param Array of trusted root certificates.
	@return Returns ETrue if the registry entry is validated against the
			trusted certificates provided.
	*/
	IMPORT_C TBool VerifyControllerSignatureL(RPointerArray<CX509Certificate>& aX509CertArray);

	/**
	The function is used to re-verify the signature and certificate of the
	SIS Controller associated to the RSisRegistryEntry object.

	aX509CertArray parameter is used to give set of trusted root certificates
	that are used for validation of the SIS Controller certificate. If the
	set given as parameter is empty, Symbian implementation can fetch the
	set of trusted root certificates from Certificate Management. If the set
	is not empty, then root certificates should not be fetched from
	Certificate Mgmt, but only the root certificates from the set given as
	parameter should be used for SIS Controller certificate validation.

	@param aX509CertArray Array of trusted root certificates.
	@param aCheckDateAndTime Indicates if the certificate validity period should be checked against the current date and time.
	@return Returns ETrue if the registry entry is validated against the
			trusted certificates provided.
	*/
	IMPORT_C TBool VerifyControllerSignatureL(RPointerArray<CX509Certificate>& aX509CertArray, TBool aCheckDateAndTime);

	/**
	Indicates whether or not the package will be removed by uninstalling the last dependant
	*/
	IMPORT_C TInt RemoveWithLastDependentL();

	/**
	This function is used to indicate the embedding package has been uninstalled but
	this package remained because of dependency.
	*/
	IMPORT_C void SetRemoveWithLastDependentL(TUid uid);

	/**
	 * @return true if the 'non-removable' flag is NOT set in the SisInfo object
	 * belonging to this package.
	 */
	IMPORT_C TBool RemovableL();
	
	/**
	 Returns set of drives to which files were installed in SIS file
	 @return Returns The bitmask of drives is as follows:
		Bit 0 -> A drive  
		Bit 1 -> B drive  
		so on and so forth.
	*/
	IMPORT_C TUint InstalledDrivesL();
	
	/**
	 * Returns whether or not the package was pre-installed
	 *
	 * @return ETrue if the package was pre-installed
	 *         EFalse otherwise
	 */
	IMPORT_C TBool PreInstalledL();
	
	/**
	* Returns whether or not the package was pre-installed and files should
	* be deleted on uninstall.  This will have been set true if and only if
	* the swipolicy indicates that preinstalled files can be deleted, and the
	* stub sis file used to install the package was writable at install time.
	*
	* @return ETrue if the package was pre-installed and files should be
	*               deleted on uninstall.
	*         EFalse otherwise
	*/
	IMPORT_C TBool IsDeletablePreInstalledL();
	
		
	/**
	* Returns the array of packages which are either dependent on this package or are
	* augmentations of this package.
	*
	* @note 
	* A is a "Dependent" of B if B is in A's dependency list
	* A is a "dependency" of B if A is in B's dependency list
	* For example consider a package Shared_library_B which is used by a package App_A. 
	* Shared_Library_B will be listed as a dependency in the sis file which installs App_A 
	* App_A cannot be installed successfully without Shared_Library_B being present. 
	* If Shared_Library_B is un-installed App_A may not function correctly.  
	* App_A is a "dependent" of Shared_library_B
	* Shared_library_B is a "dependency" of App_A
	*
	* @param aDependents The array of packages which are either dependent on this package  
	* or are augmentations of this package (i.e. its dependents)
	*
	*/
	IMPORT_C void DependentPackagesL(RPointerArray<CSisRegistryPackage>& aDependents);
	
	/**
	* Returns the dependency array for this package (ie the packages, denoted 
	* by UID and the version ranges it depends on).
	*
	* @note 
	* A is a "Dependent" of B if B is in A's dependency list
	* A is a "dependency" of B if A is in B's dependency list
	* For example consider a package Shared_library_B which is used by a package App_A. 
	* Shared_Library_B will be listed as a dependency in the sis file which installs App_A 
	* App_A cannot be installed successfully without Shared_Library_B being present. 
	* If Shared_Library_B is un-installed App_A may not function correctly.
	* In this scenario 
	* App_A is a "dependent" of Shared_library_B
	* Shared_library_B is a "dependency" of App_A
	*
	* @param aDependencies The array of packages that this package depends on (i.e. its dependencies)
	*
	*/
	IMPORT_C void DependenciesL(RPointerArray<CSisRegistryDependency>& aDependencies);

	/**
	* For a given package entry, return a array of packages that were
	* embedded within this package.
	*
	* @param aEmbedded The array of embedded packages
	*
	*/
	IMPORT_C void EmbeddedPackagesL(RPointerArray<CSisRegistryPackage>& aEmbedded);
		
	/**
	* For a given package entry, return a array of packages that 
	* embed it. 
	* Note: This is possible when package A embeds D and the subsequently installed packages
	*       B and C each in turn embed D. As there will be a single copy and a single 
	*       registration for D, it is imperative this back information is retained 
	*       and it is accessible.
	*
	* @param aEmbedding The array of embedding packages
	*
	*/
	IMPORT_C void EmbeddingPackagesL(RPointerArray<CSisRegistryPackage>& aEmbedding);
	
	/**
	* Returns ETrue if the SIS package is signed by a certificate trusted by the device (SU)
	* for eclipsing of files on the Z drive
	*
	*/
	IMPORT_C TBool IsSignedBySuCertL();

	/**
    * Returns the list of files that were created internally by registry. This function
	* is to be used to determine the list of internal files that are not to be removed for
	* an NR package during an RFS(Restore Factory Settings).
    *
    * @param aRegistryFiles     The array of filenames with complete path
    */
    IMPORT_C void RegistryFilesL(RPointerArray<HBufC>& aRegistryFiles);
		
protected:
	/**
     * @internalComponent
	 */
	HBufC8* SendReceiveBufferLC(TInt aMessage);
	/**
     * @internalComponent
	 */
	HBufC8* SendReceiveBufferLC(TInt aMessage, const TDesC& aInputDescriptor);
	};

} // namespace
#endif
