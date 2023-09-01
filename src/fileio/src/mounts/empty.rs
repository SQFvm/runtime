use crate::mount::{File, FileInfo, IOError, Mount, OpenError, PhysicalPath, ResolveError, SeekFrom, VirtualPath};

#[derive(Debug, PartialEq, Eq)]
struct EmptyFile {
}

impl File for EmptyFile {
    fn can_read(&self) -> bool {
        true
    }

    fn can_write(&self) -> bool {
        true
    }

    fn read(&mut self, buf: &mut [u8]) -> Result<usize, IOError> {
        Ok(0)
    }

    fn write(&mut self, buf: &[u8]) -> Result<(), IOError> {
        Ok(())
    }

    fn seek(&mut self, pos: SeekFrom) -> Result<(), IOError> {
        Ok(())
    }

    fn position(&mut self) -> Result<u64, IOError> {
        Ok(0)
    }

    fn flush(&mut self) -> Result<(), IOError> {
        Ok(())
    }
}

impl FileInfo for EmptyFile {
    fn is_directory(&self) -> bool {
        true
    }

    fn is_file(&self) -> bool {
        true
    }

    fn length(&self) -> Result<u64, IOError> {
        Ok(0)
    }

    fn name(&self) -> Result<String, IOError> {
        Ok(String::new())
    }

    fn path(&self) -> Result<VirtualPath, IOError> {
        Ok(String::new())
    }
}

#[derive(Debug, PartialEq, Eq)]
pub struct Empty {
    #[cfg(test)]
    pub test_data: String,
}

impl Mount for Empty {
    fn can_read(&self) -> bool {
        true
    }

    fn can_write(&self) -> bool {
        true
    }

    fn to_physical_path(&self, path: &PhysicalPath) -> Result<VirtualPath, ResolveError> {
        Ok(path.clone())
    }

    fn to_virtual_path(&self, path: &VirtualPath) -> Result<PhysicalPath, ResolveError> {
        Ok(path.clone())
    }

    fn open_read(&self, path: &VirtualPath) -> Result<Box<dyn File>, OpenError> {
        Ok(Box::new(EmptyFile{}))
    }

    fn open_write(&self, path: &VirtualPath) -> Result<Box<dyn File>, OpenError> {
        Ok(Box::new(EmptyFile{}))
    }

    fn delete(&self, path: &VirtualPath) -> Result<(), IOError> {
        Ok(())
    }

    fn create_directory(&self, path: &VirtualPath) -> Result<(), IOError> {
        Ok(())
    }

    fn create_file(&self, path: &VirtualPath) -> Result<(), IOError> {
        Ok(())
    }

    fn get_file_info(&self, path: &VirtualPath) -> Result<Box<dyn FileInfo>, IOError> {
        Ok(Box::new(EmptyFile{}))
    }
}