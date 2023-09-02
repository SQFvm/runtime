use std::fmt::{Debug, Display, Formatter};

pub type PhysicalPath = String;
pub type VirtualPath = String;

#[derive(Debug, PartialEq, Eq, )]
pub enum PropertyError {
    NotFound
}

impl Display for PropertyError {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        match self {
            PropertyError::NotFound => write!(f, "Property not found"),
        }
    }
}

impl std::error::Error for PropertyError {}

#[derive(Debug, PartialEq, Eq)]
pub enum ResolveError {}

impl Display for ResolveError {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        todo!()
    }
}

impl std::error::Error for ResolveError {}

#[derive(Debug, PartialEq, Eq)]
pub enum OpenError {}

impl Display for OpenError {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        todo!()
    }
}

impl std::error::Error for OpenError {}

#[derive(Debug, PartialEq, Eq)]
pub enum IOError {}

impl Display for IOError {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        todo!()
    }
}

impl std::error::Error for IOError {}

#[derive(Debug, PartialEq, Eq)]
pub enum SeekFrom {
    Start(u64),
    Current(i64),
    End(u64),
}

pub trait File {
    fn can_read(&self) -> bool;
    fn can_write(&self) -> bool;
    fn read(&mut self, buf: &mut [u8]) -> Result<usize, IOError>;
    fn write(&mut self, buf: &[u8]) -> Result<(), IOError>;
    fn seek(&mut self, pos: SeekFrom) -> Result<(), IOError>;
    fn position(&mut self) -> Result<u64, IOError>;
    fn flush(&mut self) -> Result<(), IOError>;
}

pub trait FileInfo {
    fn is_directory(&self) -> bool;
    fn is_file(&self) -> bool;
    fn length(&self) -> Result<u64, IOError>;
    fn name(&self) -> Result<String, IOError>;
    fn path(&self) -> Result<VirtualPath, IOError>;
}

#[derive(Debug, PartialEq, Eq, Clone, Hash)]
pub struct MountProperty {
    pub name: String,
    pub value: String,
}

pub trait Mount {
    /**
     * Gets the properties of the mount.
     *
     * # Returns
     * The properties of the mount.
     */
    fn get_properties(&self) -> Vec<MountProperty>;

    /**
     * Sets a property on the mount.
     * If the property does not exist, it will be created.
     * If the property already exists, it will be overwritten.
     * If the property cannot be set, an error will be returned.
     *
     * # Arguments
     * - `name` The name of the property to set.
     * - `value` The value of the property to set.
     * 
     * # Returns
     * An error if the property cannot be set.
     */
    fn set_property(&mut self, name: String, value: String) -> Result<(), PropertyError>;

    /**
     * Gets a property from the mount.
     * If the property does not exist, an error will be returned.
     *
     * # Arguments
     * - `name` The name of the property to get.
     * 
     * # Returns
     * The value of the property or an error if the property does not exist.
     */
    fn get_property(&self, name: String) -> Result<String, PropertyError>;

    /**
     * Returns whether the mount can read from the file system.
     * If this is false, the mount will not be able to read files.
     *
     * # Returns
     * Whether the mount can read from the file system.
     */
    fn can_read(&self) -> bool;

    /**
     * Returns whether the mount can write to the file system.
     * If this is false, the mount will not be able to create, delete or modify files.
     *
     * # Returns
     * Whether the mount can write to the file system.
     */
    fn can_write(&self) -> bool;

    /**
     * Converts a virtual path to the physical representation.
     * Eg.:
     * - `/home/user/file.txt` -> `C:\Users\user\file.txt`
     * - `/web/file.txt` -> `http://example.com/file.txt`
     *
     * # Arguments
     * - `path` The virtual path to convert.
     * 
     * # Returns
     * The physical path or an error if the path cannot be converted.
     */
    fn to_virtual_path(&self, path: &PhysicalPath) -> Result<VirtualPath, ResolveError>;

    /**
     * Converts a physical path to the virtual representation.
     * Eg.:
     * - `C:\Users\user\file.txt` -> `/home/user/file.txt`
     * - `http://example.com/file.txt` -> `/web/file.txt`
     *
     * # Arguments
     * - `path` The physical path to convert.
     * 
     * # Returns
     * The virtual path or an error if the path cannot be converted.
     */
    fn to_physical_path(&self, path: &VirtualPath) -> Result<PhysicalPath, ResolveError>;

    /**
     * Opens a file for reading or writing.
     * If the file does not exist, it will not be created and an error will be returned.
     *
     * # Arguments
     * - `path` The path to the file to open.
     * 
     * # Returns
     * The file or an error if the file cannot be opened.
     */
    fn open_read(&self, path: &VirtualPath) -> Result<Box<dyn File>, OpenError>;

    /**
     * Opens a file for writing.
     * If the file does not exist, it will not be created and an error will be returned.
     *
     * # Arguments
     * - `path` The path to the file to open.
     * 
     * # Returns
     * The file or an error if the file cannot be opened.
     */
    fn open_write(&self, path: &VirtualPath) -> Result<Box<dyn File>, OpenError>;

    /**
     * Deletes a file.
     * If the file does not exist, an error will be returned.
     *
     * # Arguments
     * - `path` The path to the file to delete.
     * 
     * # Returns
     * The file or an error if the file cannot be deleted.
     */
    fn delete(&self, path: &VirtualPath) -> Result<(), IOError>;

    /**
     * Creates a directory.
     * If the directory already exists, an error will be returned.
     *
     * # Arguments
     * - `path` The path to the directory to create.
     * 
     * # Returns
     * The directory or an error if the directory cannot be created.
     */
    fn create_directory(&self, path: &VirtualPath) -> Result<Box<dyn FileInfo>, IOError>;

    /**
     * Creates a file.
     * If the file already exists, an error will be returned.
     *
     * # Arguments
     * - `path` The path to the file to create.
     * 
     * # Returns
     * The file or an error if the file cannot be created.
     */
    fn create_file(&self, path: &VirtualPath) -> Result<Box<dyn FileInfo>, IOError>;

    /**
     * Gets information about a file.
     * If the file does not exist, an error will be returned.
     *
     * # Arguments
     * - `path` The path to the file to get information about.
     * 
     * # Returns
     * The file or an error if the file cannot be found.
     */
    fn get_file_info(&self, path: &VirtualPath) -> Result<Box<dyn FileInfo>, IOError>;
}

impl Debug for dyn Mount {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        f.debug_struct("Mount")
            .field("properties", &self.get_properties())
            .field("can_read", &self.can_read())
            .field("can_write", &self.can_write())
            .finish()
    }
}