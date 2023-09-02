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

    /**
     * Sanitize a path.
     *
     * # Arguments
     * - `path` The path to sanitize.
     *
     * # Returns
     * The sanitized path.
     */
    fn sanitize_path(path: &VirtualPath) -> VirtualPath {
        let mut sanitized = path.clone();
        sanitized = sanitized.replace("\\", "/");
        while sanitized.contains("//") {
            sanitized = sanitized.replace("//", "/");
        }
        if sanitized.ends_with("/") {
            sanitized.pop();
        }
        if !sanitized.starts_with("/") {
            sanitized.insert(0, '/');
        }
        sanitized
    }

    /**
     * Create a new file system.
     *
     * # Returns
     * The new file system.
     */
    pub fn new() -> VirtualFileSystem {
        VirtualFileSystem {
            mounts: Vec::new(),
        }
    }

    /**
     * Mount a mount.
     *
     * # Arguments
     * - `path` The virtual path to mount the mount at.
     * - `mount` The mount to mount.
     * 
     * # Returns
     * Whether the mount was mounted.
     */
    pub fn mount(&mut self, path: VirtualMountPath, mount: Box<dyn Mount>) -> MountResult {
        let path = Self::sanitize_path(&path);
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

    /**
     * Unmount a mount.
     *
     * # Arguments
     * - `path` The virtual path of the mount to unmount.
     * 
     * # Returns
     * Whether the mount was unmounted.
     */
    pub fn unmount(&mut self, path: VirtualMountPath) -> UnmountResult {
        let path = Self::sanitize_path(&path);
        if let Some(index) = self.mounts.iter().position(|entry| entry.path == path) {
            self.mounts.remove(index);
            UnmountResult::Success
        } else {
            UnmountResult::NotMounted
        }
    }

    /**
     * Resolve a virtual path to a mount and a path relative to that mount.
     *
     * # Arguments
     * - `path` The full virtual path to resolve.
     * 
     * # Returns
     * A tuple containing the mount and the remaining path relative to that mount.
     */
    pub fn resolve<'a>(&'a self, path: &VirtualPath) -> Result<(&'a Box<dyn Mount>, VirtualPath), ResolveError> {
        let path = Self::sanitize_path(&path);
        let mut mount_path = "";
        let mut mount = None;
        for entry in self.mounts.iter() {
            if path.starts_with(&entry.path) && entry.path.len() > mount_path.len() {
                mount_path = &entry.path;
                mount = Some(&entry.mount);
            }
        }
        if let Some(mount) = mount {
            Ok((mount, Self::sanitize_path(&path[mount_path.len()..].to_string())))
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
    use crate::mount::Mount;
    use crate::mounts::Empty;


    #[test]
    #[traced_test]
    fn mount_with_root_wokrs() -> Result<(), Box<dyn std::error::Error>> {
        let mut file_system = super::VirtualFileSystem::new();
        let result = file_system.mount("/".to_string(), Box::new(Empty::new()));
        assert_eq!(result, super::MountResult::Success);
        Ok(())
    }

    #[test]
    #[traced_test]
    fn mount_with_root_twice_fails() -> Result<(), Box<dyn std::error::Error>> {
        let mut file_system = super::VirtualFileSystem::new();
        let result = file_system.mount("/".to_string(), Box::new(Empty::new()));
        assert_eq!(result, super::MountResult::Success);
        let result = file_system.mount("/".to_string(), Box::new(Empty::new()));
        assert_eq!(result, super::MountResult::AlreadyMounted);
        Ok(())
    }

    #[test]
    #[traced_test]
    fn mount_with_overlapping_roots_works() -> Result<(), Box<dyn std::error::Error>> {
        let mut file_system = super::VirtualFileSystem::new();
        let result = file_system.mount("/".to_string(), Box::new(Empty::new()));
        assert_eq!(result, super::MountResult::Success);
        let result = file_system.mount("/test".to_string(), Box::new(Empty::new()));
        assert_eq!(result, super::MountResult::Success);
        Ok(())
    }

    #[test]
    #[traced_test]
    fn mount_with_different_paths_but_no_root_mount_works() -> Result<(), Box<dyn std::error::Error>> {
        let mut file_system = super::VirtualFileSystem::new();
        let result = file_system.mount("/test".to_string(), Box::new(Empty::new()));
        assert_eq!(result, super::MountResult::Success);
        let result = file_system.mount("/test2".to_string(), Box::new(Empty::new()));
        assert_eq!(result, super::MountResult::Success);
        Ok(())
    }

    #[test]
    #[traced_test]
    fn unmount_with_root_mounted_works() -> Result<(), Box<dyn std::error::Error>> {
        let mut file_system = super::VirtualFileSystem::new();
        let result = file_system.mount("/".to_string(), Box::new(Empty::new()));
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
        let result = file_system.mount("/test".to_string(), Box::new(Empty::new()));
        assert_eq!(result, super::MountResult::Success);
        let result = file_system.unmount("/test2".to_string());
        assert_eq!(result, super::UnmountResult::NotMounted);
        Ok(())
    }

    #[test]
    #[traced_test]
    fn unmount_with_something_mounted_on_non_root_will_fail_if_root_is_not_mounted() -> Result<(), Box<dyn std::error::Error>> {
        let mut file_system = super::VirtualFileSystem::new();
        let result = file_system.mount("/test".to_string(), Box::new(Empty::new()));
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
        file_system.mount("/".to_string(), Box::new(Empty::new()));
        let result = file_system.resolve(&"/".to_string());
        assert!(result.is_ok());
        let (mount, path) = result.unwrap();
        assert_eq!(path, "/".to_string());
        Ok(())
    }

    #[test]
    #[traced_test]
    fn resolve_with_empty_mounted_on_root_and_path_yields_empty_mounted_and_path() -> Result<(), Box<dyn std::error::Error>> {
        let mut file_system = super::VirtualFileSystem::new();
        file_system.mount("/".to_string(), Box::new(Empty::new()));
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
        file_system.mount("/mount/".to_string(), Box::new(Empty::new()));
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

        let mut outer = Empty::new();
        assert_eq!(outer.set_property("test".to_string(), "outer".to_string()), Ok(()));
        file_system.mount("/mount/".to_string(), Box::new(outer));

        let mut inner = Empty::new();
        assert_eq!(inner.set_property("test".to_string(), "inner".to_string()), Ok(()));
        file_system.mount("/mount/test/".to_string(), Box::new(inner));

        let result = file_system.resolve(&"/mount/outter".to_string());
        assert!(result.is_ok());
        let (mount, path) = result.unwrap();
        assert_eq!(path, "/outter".to_string());
        assert_eq!(mount.get_property("test".to_string()), Ok("outer".to_string()));

        let result = file_system.resolve(&"/mount/test/inner".to_string());
        assert!(result.is_ok());
        let (mount, path) = result.unwrap();
        assert_eq!(path, "/inner".to_string());
        assert_eq!(mount.get_property("test".to_string()), Ok("inner".to_string()));
        Ok(())
    }

    #[test]
    #[traced_test]
    fn resolve_with_overlapping_mounts_yields_more_narrow_mount_with_correct_subpath_and_mount_order_does_not_matter() -> Result<(), Box<dyn std::error::Error>> {
        let mut file_system = super::VirtualFileSystem::new();

        let mut inner = Empty::new();
        assert_eq!(inner.set_property("test".to_string(), "inner".to_string()), Ok(()));
        file_system.mount("/mount/test/".to_string(), Box::new(inner));

        let mut outer = Empty::new();
        assert_eq!(outer.set_property("test".to_string(), "outer".to_string()), Ok(()));
        file_system.mount("/mount/".to_string(), Box::new(outer));

        let result = file_system.resolve(&"/mount/outter".to_string());
        assert!(result.is_ok());
        let (mount, path) = result.unwrap();
        assert_eq!(path, "/outter".to_string());
        assert_eq!(mount.get_property("test".to_string()), Ok("outer".to_string()));

        let result = file_system.resolve(&"/mount/test/inner".to_string());
        assert!(result.is_ok());
        let (mount, path) = result.unwrap();
        assert_eq!(path, "/inner".to_string());
        assert_eq!(mount.get_property("test".to_string()), Ok("inner".to_string()));
        Ok(())
    }

    #[test]
    #[traced_test]
    fn resolve_neither_forward_nor_backward_slash_matters( ) -> Result<(), Box<dyn std::error::Error>> {
        let mut file_system = super::VirtualFileSystem::new();

        let mut inner = Empty::new();
        assert_eq!(inner.set_property("test".to_string(), "inner".to_string()), Ok(()));
        file_system.mount("/mount/test/".to_string(), Box::new(inner));

        let mut outer = Empty::new();
        assert_eq!(outer.set_property("test".to_string(), "outer".to_string()), Ok(()));
        file_system.mount("/mount/".to_string(), Box::new(outer));

        let result = file_system.resolve(&"/mount/outter".to_string());
        assert!(result.is_ok());
        let (mount, path) = result.unwrap();
        assert_eq!(path, "/outter".to_string());
        assert_eq!(mount.get_property("test".to_string()), Ok("outer".to_string()));

        let result = file_system.resolve(&"/mount/test/inner".to_string());
        assert!(result.is_ok());
        let (mount, path) = result.unwrap();
        assert_eq!(path, "/inner".to_string());
        assert_eq!(mount.get_property("test".to_string()), Ok("inner".to_string()));

        let result = file_system.resolve(&"\\mount\\outter".to_string());
        assert!(result.is_ok());
        let (mount, path) = result.unwrap();
        assert_eq!(path, "/outter".to_string());
        assert_eq!(mount.get_property("test".to_string()), Ok("outer".to_string()));

        let result = file_system.resolve(&"\\mount\\test\\inner".to_string());
        assert!(result.is_ok());
        let (mount, path) = result.unwrap();
        assert_eq!(path, "/inner".to_string());
        assert_eq!(mount.get_property("test".to_string()), Ok("inner".to_string()));
        Ok(())
    }
}