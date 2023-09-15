use std::io::{Read, Seek, Write};
use std::io::SeekFrom::{Current, End, Start};
use std::path::Path;
use crate::mount::{File, FileInfo, IOError, Mount, MountProperty, OpenError, PhysicalPath, PropertyError, ResolveError, SeekFrom, VirtualPath};

struct FileSystemFile {
    path: PhysicalPath,
    file: std::fs::File,
}

impl File for FileSystemFile {
    fn can_read(&self) -> bool {
        true
    }

    fn can_write(&self) -> bool {
        true
    }

    fn read(&mut self, buf: &mut [u8]) -> Result<usize, IOError> {
        let result = self.file.read(buf);
        match result {
            Ok(size) => Ok(size),
            Err(_) => Err(IOError::ReadFailed),
        }
    }

    fn write(&mut self, buf: &[u8]) -> Result<(), IOError> {
        let result = self.file.write(buf);
        match result {
            Ok(_) => Ok(()),
            Err(_) => Err(IOError::WriteFailed),
        }
    }

    fn seek(&mut self, pos: SeekFrom) -> Result<(), IOError> {
        let result = self.file.seek(match pos {
            SeekFrom::Start(start) => Start(start),
            SeekFrom::Current(current) => Current(current),
            SeekFrom::End(end) => End(end),
        });
        match result {
            Ok(_) => Ok(()),
            Err(_) => Err(IOError::SeekFailed),
        }
    }

    fn position(&mut self) -> Result<u64, IOError> {
        let result = self.file.stream_position();
        match result {
            Ok(pos) => Ok(pos),
            Err(_) => Err(IOError::PositionFailed),
        }
    }

    fn flush(&mut self) -> Result<(), IOError> {
        let result = self.file.flush();
        match result {
            Ok(_) => Ok(()),
            Err(_) => Err(IOError::FlushFailed),
        }
    }
}

#[derive(Debug, PartialEq, Eq)]
struct FileSystemFileInfo<'a> {
    virtual_path: &'a VirtualPath,
    physical_path: &'a Path,
}
impl FileInfo for FileSystemFileInfo<'_> {
    fn is_directory(&self) -> bool {
        let result = std::fs::metadata(&self.physical_path);
        match result {
            Ok(metadata) => metadata.is_dir(),
            Err(_) => false,
        }
    }

    fn is_file(&self) -> bool {
        let result = std::fs::metadata(&self.physical_path);
        match result {
            Ok(metadata) => metadata.is_file(),
            Err(_) => false,
        }
    }

    fn length(&self) -> Result<u64, IOError> {
        let result = std::fs::metadata(&self.physical_path);
        match result {
            Ok(metadata) => Ok(metadata.len()),
            Err(_) => Err(IOError::ReadFailed),
        }
    }

    fn name(&self) -> Result<String, IOError> {
        match self.physical_path.file_name() {
            Some(name) => match name.to_str()
            {
                Some(name) => Ok(name.to_string()),
                None => Err(IOError::FailedToGetFileName),
            },
            None => Err(IOError::FailedToGetFileName),
        }
    }

    fn virtual_path(&self) -> Result<&VirtualPath, IOError> {
        Ok(self.virtual_path.clone())
    }
    fn physical_path(&self) -> Result<&PhysicalPath, IOError> {
        Ok(&self.physical_path.as_os_str().to_os_string())
    }
}

#[derive(Debug, PartialEq, Eq)]
pub struct FileSystem {
    properties: Vec<MountProperty>,
}

impl FileSystem {
    pub fn new() -> FileSystem {
        FileSystem { properties: vec![] }
    }
}

impl Mount for FileSystem {
    fn get_properties(&self) -> Vec<MountProperty> {
        self.properties.clone()
    }

    fn set_property(&mut self, name: String, value: String) -> Result<(), PropertyError> {
        if let Some(property) = self.properties.iter_mut().find(|p| p.name == name) {
            property.value = value;
        } else {
            self.properties.push(MountProperty { name, value });
        }
        Ok(())
    }

    fn get_property(&self, name: String) -> Result<String, PropertyError> {
        if let Some(property) = self.properties.iter().find(|p| p.name == name) {
            Ok(property.value.clone())
        } else {
            Err(PropertyError::NotFound)
        }
    }

    fn can_read(&self) -> bool {
        true
    }

    fn can_write(&self) -> bool {
        true
    }

    fn to_virtual_path(&self, path: &PhysicalPath) -> Result<&VirtualPath, ResolveError> {
        todo!()
    }

    fn to_physical_path(&self, path: &VirtualPath) -> Result<&PhysicalPath, ResolveError> {
        todo!()
    }

    fn open_read<'a>(&self, path: &VirtualPath) -> Result<&'a dyn File, OpenError> {
        todo!()
    }

    fn open_write<'a>(&self, path: &VirtualPath) -> Result<&'a dyn File, OpenError> {
        todo!()
    }

    fn delete(&self, path: &VirtualPath) -> Result<(), IOError> {
        todo!()
    }

    fn create_directory<'a>(&self, path: &VirtualPath) -> Result<(), IOError> {
        todo!()
    }

    fn create_file<'a>(&self, path: &VirtualPath) -> Result<&'a dyn File, IOError> {
        todo!()
    }

    fn get_file_info<'a>(&self, path: &VirtualPath) -> Result<&'a dyn FileInfo, IOError> {
        todo!()
    }

    fn iter_directory<'a>(&self, path: &VirtualPath, recursive: bool) -> Result<&'a dyn Iterator<Item=&'a dyn FileInfo>, IOError> {
        todo!()
    }
}


#[cfg(test)]
mod tests {
    use tracing_test::traced_test;
    use crate::mount::Mount;
    use crate::mounts::Empty;

}