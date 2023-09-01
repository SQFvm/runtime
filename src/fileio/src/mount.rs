use std::fmt::{Debug, Formatter};

pub type PhysicalPath = String;
pub type VirtualPath = String;

#[derive(Debug, PartialEq, Eq)]
pub enum ResolveError {
}

#[derive(Debug, PartialEq, Eq)]
pub enum OpenError {
}

#[derive(Debug, PartialEq, Eq)]
pub enum IOError {
}

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

pub trait Mount {
    fn can_read(&self) -> bool;
    fn can_write(&self) -> bool;
    fn to_physical_path(&self, path: &PhysicalPath) -> Result<VirtualPath, ResolveError>;
    fn to_virtual_path(&self, path: &VirtualPath) -> Result<PhysicalPath, ResolveError>;
    fn open_read(&self, path: &VirtualPath) -> Result<Box<dyn File>, OpenError>;
    fn open_write(&self, path: &VirtualPath) -> Result<Box<dyn File>, OpenError>;
    fn delete(&self, path: &VirtualPath) -> Result<(), IOError>;
    fn create_directory(&self, path: &VirtualPath) -> Result<(), IOError>;
    fn create_file(&self, path: &VirtualPath) -> Result<(), IOError>;
    fn get_file_info(&self, path: &VirtualPath) -> Result<Box<dyn FileInfo>, IOError>;
    
    #[cfg(test)]
    fn as_any(&self) -> &dyn std::any::Any{
        self
    }
}

impl Debug for dyn Mount {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        write!(f, "Mount")
    }
}