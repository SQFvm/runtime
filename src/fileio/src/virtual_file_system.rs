use crate::mount::{Mount, VirtualPath};

pub type VirtualMountPath = String;

#[derive(Debug, PartialEq, Eq)]
pub enum MountResult {
    Success,
    AlreadyMounted,
}

#[derive(Debug, PartialEq, Eq)]
pub enum UnmountResult {
    Success,
    NotMounted,
}

#[derive(Debug, PartialEq, Eq)]
pub enum ResolveError {
    NothingMounted,
    PathNotPartOfMount,
}

struct MountEntry {
    mount: Box<dyn Mount>,
    path: VirtualMountPath,
}

pub struct VirtualFileSystem {
    mounts: Vec<MountEntry>,
}

impl VirtualFileSystem {
    /*
     * Create a new file system.
     *
     * @return The new file system.
     */
    pub fn new() -> VirtualFileSystem {
        VirtualFileSystem {
            mounts: Vec::new(),
        }
    }

    /*
     * Mount a mount.
     *
     * @param path The virtual path to mount the mount at.
     * @param mount The mount to mount.
     * @return Whether the mount was mounted.
     */
    pub fn mount(&mut self, path: VirtualMountPath, mount: Box<dyn Mount>) -> MountResult {
        if self.mounts.iter().any(|entry| entry.path == path) {
            MountResult::AlreadyMounted
        } else {
            self.mounts.push(MountEntry {
                mount,
                path,
            });
            MountResult::Success
        }
    }

    /*
     * Unmount a mount.
     *
     * @param path The virtual path of the mount to unmount.
     * @return Whether the mount was unmounted.
     */
    pub fn unmount(&mut self, path: VirtualMountPath) -> UnmountResult {
        if let Some(index) = self.mounts.iter().position(|entry| entry.path == path) {
            self.mounts.remove(index);
            UnmountResult::Success
        } else {
            UnmountResult::NotMounted
        }
    }

    /*
     * Resolve a virtual path to a mount and a path relative to that mount.
     *
     * @param path The full virtual path to resolve.
     * @return A tuple containing the mount and the remaining path relative to that mount.
     */
    pub fn resolve<'a>(&'a self, path: &VirtualPath) -> Result<(&'a Box<dyn Mount>, VirtualPath), ResolveError> {
        let mut mount_path = "";
        let mut mount = None;
        for entry in self.mounts.iter() {
            if path.starts_with(&entry.path) && entry.path.len() > mount_path.len() {
                mount_path = &entry.path;
                mount = Some(&entry.mount);
            }
        }
        if let Some(mount) = mount {
            Ok((mount, path[mount_path.len()..].to_string()))
        } else if self.mounts.is_empty() {
            Err(ResolveError::NothingMounted)
        } else {
            Err(ResolveError::PathNotPartOfMount)
        }
    }
}


#[cfg(test)]
mod tests {
    use tracing_test::traced_test;
    use crate::mounts::Empty;


    #[test]
    #[traced_test]
    fn mount_with_root_wokrs() -> Result<(), Box<dyn std::error::Error>> {
        let mut file_system = super::VirtualFileSystem::new();
        let result = file_system.mount("/".to_string(), Box::new(Empty {test_data: "".to_string()}));
        assert_eq!(result, super::MountResult::Success);
        Ok(())
    }

    #[test]
    #[traced_test]
    fn mount_with_root_twice_fails() -> Result<(), Box<dyn std::error::Error>> {
        let mut file_system = super::VirtualFileSystem::new();
        let result = file_system.mount("/".to_string(), Box::new(Empty {test_data: "".to_string()}));
        assert_eq!(result, super::MountResult::Success);
        let result = file_system.mount("/".to_string(), Box::new(Empty {test_data: "".to_string()}));
        assert_eq!(result, super::MountResult::AlreadyMounted);
        Ok(())
    }

    #[test]
    #[traced_test]
    fn mount_with_overlapping_roots_works() -> Result<(), Box<dyn std::error::Error>> {
        let mut file_system = super::VirtualFileSystem::new();
        let result = file_system.mount("/".to_string(), Box::new(Empty {test_data: "".to_string()}));
        assert_eq!(result, super::MountResult::Success);
        let result = file_system.mount("/test".to_string(), Box::new(Empty {test_data: "".to_string()}));
        assert_eq!(result, super::MountResult::Success);
        Ok(())
    }

    #[test]
    #[traced_test]
    fn mount_with_different_paths_but_no_root_mount_works() -> Result<(), Box<dyn std::error::Error>> {
        let mut file_system = super::VirtualFileSystem::new();
        let result = file_system.mount("/test".to_string(), Box::new(Empty {test_data: "".to_string()}));
        assert_eq!(result, super::MountResult::Success);
        let result = file_system.mount("/test2".to_string(), Box::new(Empty {test_data: "".to_string()}));
        assert_eq!(result, super::MountResult::Success);
        Ok(())
    }

    #[test]
    #[traced_test]
    fn unmount_with_root_mounted_works() -> Result<(), Box<dyn std::error::Error>> {
        let mut file_system = super::VirtualFileSystem::new();
        let result = file_system.mount("/".to_string(), Box::new(Empty {test_data: "".to_string()}));
        assert_eq!(result, super::MountResult::Success);
        let result = file_system.unmount("/".to_string());
        assert_eq!(result, super::UnmountResult::Success);
        Ok(())
    }

    #[test]
    #[traced_test]
    fn unmount_with_nothing_mounted_fails() -> Result<(), Box<dyn std::error::Error>> {
        let mut file_system = super::VirtualFileSystem::new();
        let result = file_system.unmount("/".to_string());
        assert_eq!(result, super::UnmountResult::NotMounted);
        Ok(())
    }

    #[test]
    #[traced_test]
    fn unmount_with_something_mounted_at_different_path_fails() -> Result<(), Box<dyn std::error::Error>> {
        let mut file_system = super::VirtualFileSystem::new();
        let result = file_system.mount("/test".to_string(), Box::new(Empty {test_data: "".to_string()}));
        assert_eq!(result, super::MountResult::Success);
        let result = file_system.unmount("/test2".to_string());
        assert_eq!(result, super::UnmountResult::NotMounted);
        Ok(())
    }

    #[test]
    #[traced_test]
    fn unmount_with_something_mounted_on_non_root_will_fail_if_root_is_not_mounted() -> Result<(), Box<dyn std::error::Error>> {
        let mut file_system = super::VirtualFileSystem::new();
        let result = file_system.mount("/test".to_string(), Box::new(Empty {test_data: "".to_string()}));
        assert_eq!(result, super::MountResult::Success);
        let result = file_system.unmount("/".to_string());
        assert_eq!(result, super::UnmountResult::NotMounted);
        Ok(())
    }

    #[test]
    #[traced_test]
    fn resolve_with_nothing_mounted_yields_nothing_mounted_error() -> Result<(), Box<dyn std::error::Error>> {
        let file_system = super::VirtualFileSystem::new();
        let result = file_system.resolve(&"/".to_string());
        assert!(result.is_err());
        assert!(result.is_err() && result.unwrap_err() == super::ResolveError::NothingMounted);
        Ok(())
    }

    #[test]
    #[traced_test]
    fn resolve_with_empty_mounted_yields_empty_mounted() -> Result<(), Box<dyn std::error::Error>> {
        let mut file_system = super::VirtualFileSystem::new();
        file_system.mount("/".to_string(), Box::new(Empty {test_data: "".to_string()}));
        let result = file_system.resolve(&"/".to_string());
        assert!(result.is_ok());
        let (mount, path) = result.unwrap();
        assert_eq!(path, "".to_string());
        Ok(())
    }

    #[test]
    #[traced_test]
    fn resolve_with_empty_mounted_on_root_and_path_yields_empty_mounted_and_path() -> Result<(), Box<dyn std::error::Error>> {
        let mut file_system = super::VirtualFileSystem::new();
        file_system.mount("/".to_string(), Box::new(Empty {test_data: "".to_string()}));
        let result = file_system.resolve(&"/test".to_string());
        assert!(result.is_ok());
        let (mount, path) = result.unwrap();
        assert_eq!(path, "/test".to_string());
        Ok(())
    }

    #[test]
    #[traced_test]
    fn resolve_with_empty_mounted_on_path_yields_empty_mounted_and_subpath() -> Result<(), Box<dyn std::error::Error>> {
        let mut file_system = super::VirtualFileSystem::new();
        file_system.mount("/mount/".to_string(), Box::new(Empty {test_data: "".to_string()}));
        let result = file_system.resolve(&"/mount/test/subtest".to_string());
        assert!(result.is_ok());
        let (mount, path) = result.unwrap();
        assert_eq!(path, "/test/subtest".to_string());
        Ok(())
    }

    #[test]
    #[traced_test]
    fn resolve_with_overlapping_mounts_yields_more_narrow_mount_with_correct_subpath() -> Result<(), Box<dyn std::error::Error>> {
        let mut file_system = super::VirtualFileSystem::new();
        file_system.mount("/mount/".to_string(), Box::new(Empty { test_data: "outer".to_string()}));
        file_system.mount("/mount/test/".to_string(), Box::new(Empty { test_data: "inner".to_string()}));
        let result = file_system.resolve(&"/mount/outter".to_string());
        assert!(result.is_ok());
        let (mount, path) = result.unwrap();
        assert_eq!(path, "/outter".to_string());
        assert_eq!(mount.as_any().downcast_mut::<Empty>(), Some(&mut Empty { test_data: "outer".to_string()}));

        let result = file_system.resolve(&"/mount/test/inner".to_string());
        assert!(result.is_ok());
        let (mount, path) = result.unwrap();
        assert_eq!(path, "/inner".to_string());
        assert_eq!(mount.as_any().downcast_mut::<Empty>(), Some(&mut Empty { test_data: "inner".to_string()}));
        Ok(())
    }

    #[test]
    #[traced_test]
    fn resolve_with_overlapping_mounts_yields_more_narrow_mount_with_correct_subpath_and_mount_order_does_not_matter() -> Result<(), Box<dyn std::error::Error>> {
        let mut file_system = super::VirtualFileSystem::new();
        file_system.mount("/mount/test/".to_string(), Box::new(Empty { test_data: "inner".to_string()}));
        file_system.mount("/mount/".to_string(), Box::new(Empty { test_data: "outer".to_string()}));
        let result = file_system.resolve(&"/mount/outter".to_string());
        assert!(result.is_ok());
        let (mount, path) = result.unwrap();
        assert_eq!(path, "/outter".to_string());
        assert_eq!(mount.as_any().downcast_mut::<Empty>(), Some(&mut Empty { test_data: "outer".to_string()}));

        let result = file_system.resolve(&"/mount/test/inner".to_string());
        assert!(result.is_ok());
        let (mount, path) = result.unwrap();
        assert_eq!(path, "/inner".to_string());
        assert_eq!(mount.as_any().downcast_mut::<Empty>(), Some(&mut Empty { test_data: "inner".to_string()}));
        Ok(())
    }
}