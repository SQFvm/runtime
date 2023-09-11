use crate::mount::{File, FileInfo, IOError, Mount, MountProperty, OpenError, PhysicalPath, PropertyError, ResolveError, SeekFrom, VirtualPath};
use crate::path::Path;

#[derive(Debug, PartialEq, Eq)]
struct EmptyFile {}

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

    fn virtual_path(&self) -> Result<VirtualPath, IOError> {
        Ok(Path::empty())
    }

    fn physical_path(&self) -> Result<PhysicalPath, IOError> {
        Ok(Path::empty())
    }
}

#[derive(Debug, PartialEq, Eq)]
pub struct Empty {
    properties: Vec<MountProperty>,
}

impl Empty {
    pub fn new() -> Empty {
        Empty { properties: vec![] }
    }
}

impl Mount for Empty {
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

    fn to_virtual_path(&self, path: &PhysicalPath) -> Result<VirtualPath, ResolveError> {
        Ok(path.clone())
    }

    fn to_physical_path(&self, path: &VirtualPath) -> Result<PhysicalPath, ResolveError> {
        Ok(path.clone())
    }

    fn open_read(&self, path: &VirtualPath) -> Result<Box<dyn File>, OpenError> {
        Ok(Box::new(EmptyFile {}))
    }

    fn open_write(&self, path: &VirtualPath) -> Result<Box<dyn File>, OpenError> {
        Ok(Box::new(EmptyFile {}))
    }

    fn delete(&self, path: &VirtualPath) -> Result<(), IOError> {
        Ok(())
    }

    fn create_directory(&self, path: &VirtualPath) -> Result<Box<dyn FileInfo>, IOError> {
        Ok(Box::new(EmptyFile {}))
    }

    fn create_file(&self, path: &VirtualPath) -> Result<Box<dyn FileInfo>, IOError> {
        Ok(Box::new(EmptyFile {}))
    }

    fn get_file_info(&self, path: &VirtualPath) -> Result<Box<dyn FileInfo>, IOError> {
        Ok(Box::new(EmptyFile {}))
    }

    fn iter_directory(&self, path: &VirtualPath, recursive: bool) -> Result<Box<dyn Iterator<Item=Box<dyn FileInfo>>>, IOError> {
        Ok(Box::new(std::iter::empty()))
    }
}


#[cfg(test)]
mod tests {
    use tracing_test::traced_test;
    use crate::mount::Mount;
    use crate::mounts::Empty;


    #[test]
    #[traced_test]
    fn get_properties_returns_empty_vec_if_no_properties_are_set() -> Result<(), Box<dyn std::error::Error>> {
        let empty = Empty { properties: vec![] };
        assert_eq!(empty.get_properties(), vec![]);
        Ok(())
    }

    #[test]
    #[traced_test]
    fn get_properties_returns_vec_of_properties_if_properties_are_set() -> Result<(), Box<dyn std::error::Error>> {
        let mut empty = Empty { properties: vec![] };
        empty.set_property("foo".to_string(), "bar".to_string())?;
        assert_eq!(empty.get_properties(), vec![crate::mount::MountProperty { name: "foo".to_string(), value: "bar".to_string() }]);
        Ok(())
    }

    #[test]
    #[traced_test]
    fn set_property_sets_property_if_property_does_not_exist() -> Result<(), Box<dyn std::error::Error>> {
        let mut empty = Empty { properties: vec![] };
        empty.set_property("foo".to_string(), "bar".to_string())?;
        assert_eq!(empty.get_properties(), vec![crate::mount::MountProperty { name: "foo".to_string(), value: "bar".to_string() }]);
        empty.set_property("bar".to_string(), "foo".to_string())?;
        assert_eq!(empty.get_properties(), vec![crate::mount::MountProperty { name: "foo".to_string(), value: "bar".to_string() }, crate::mount::MountProperty { name: "bar".to_string(), value: "foo".to_string() }]);
        Ok(())
    }

    #[test]
    #[traced_test]
    fn set_property_overwrites_property_if_property_exists() -> Result<(), Box<dyn std::error::Error>> {
        let mut empty = Empty { properties: vec![] };
        empty.set_property("foo".to_string(), "bar".to_string())?;
        assert_eq!(empty.get_properties(), vec![crate::mount::MountProperty { name: "foo".to_string(), value: "bar".to_string() }]);
        empty.set_property("foo".to_string(), "foo".to_string())?;
        assert_eq!(empty.get_properties(), vec![crate::mount::MountProperty { name: "foo".to_string(), value: "foo".to_string() }]);
        Ok(())
    }

    #[test]
    #[traced_test]
    fn get_property_returns_error_if_property_does_not_exist() -> Result<(), Box<dyn std::error::Error>> {
        let empty = Empty { properties: vec![] };
        assert_eq!(empty.get_property("foo".to_string()), Err(crate::mount::PropertyError::NotFound));
        Ok(())
    }

    #[test]
    #[traced_test]
    fn get_property_returns_property_if_property_exists() -> Result<(), Box<dyn std::error::Error>> {
        let mut empty = Empty { properties: vec![] };
        empty.set_property("foo".to_string(), "bar".to_string())?;
        assert_eq!(empty.get_property("foo".to_string()), Ok("bar".to_string()));
        Ok(())
    }
}