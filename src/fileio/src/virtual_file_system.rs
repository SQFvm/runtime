use std::error::Error;
use std::fmt::{Display, Formatter};
use std::path::Path;
use crate::mount::{Mount, VirtualPath};

pub type VirtualMountPath = Path;

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
    PathStrippingFailed,
}

impl Display for ResolveError {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        match self {
            ResolveError::NothingMounted => write!(f, "Nothing mounted"),
            ResolveError::PathNotPartOfMount => write!(f, "Path not part of mount"),
            ResolveError::PathStrippingFailed => write!(f, "Path stripping failed"),
        }
    }
}

impl Error for ResolveError {}

struct MountEntry<'a> {
    mount: &'a dyn Mount,
    path: &'a VirtualMountPath,
}

pub struct VirtualFileSystem<'a> {
    mounts: Vec<MountEntry<'a>>,
}

impl VirtualFileSystem<'_> {
    /**
     * Create a new file system.
     *
     * # Returns
     * The new file system.
     */
    pub fn new<'a>() -> VirtualFileSystem<'a> {
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
    pub fn mount(&'_ mut self, path: &VirtualMountPath, mount: &'_ dyn Mount) -> MountResult {
        if self.mounts.iter().any(|entry| entry.path == path) {
            MountResult::AlreadyMounted
        } else {
            self.mounts.push(MountEntry {
                mount,
                path: path.clone(),
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
    pub fn unmount(&mut self, path: &VirtualMountPath) -> UnmountResult {
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
    pub fn resolve(&'_ self, path: &VirtualPath) -> Result<(&'_ dyn Mount, &VirtualPath), ResolveError> {
        let mut mount_path = VirtualMountPath::new("/");
        let mut mount = None;
        for entry in self.mounts.iter() {
            if path.starts_with(&entry.path) && entry.path.as_os_str().len() > mount_path.as_os_str().len() {
                mount_path = entry.path;
                mount = Some(entry.mount);
            }
        }
        if let Some(mount) = mount {
            match path.strip_prefix(mount_path) {
                Ok(path) => Ok((mount, path)),
                Err(_) => Err(ResolveError::PathStrippingFailed),
            }
        } else if self.mounts.is_empty() {
            Err(ResolveError::NothingMounted)
        } else {
            Err(ResolveError::PathNotPartOfMount)
        }
    }
}


#[cfg(test)]
mod tests {
    use std::path::Path;
    use tracing_test::traced_test;
    use crate::mount::Mount;
    use crate::mounts::Empty;


    #[test]
    #[traced_test]
    fn mount_with_root_wokrs() -> Result<(), Box<dyn std::error::Error>> {
        let mut file_system = super::VirtualFileSystem::new();
        let result = file_system.mount(Path::new("/"), Empty::new());
        assert_eq!(result, super::MountResult::Success);
        Ok(())
    }

    #[test]
    #[traced_test]
    fn mount_with_root_twice_fails() -> Result<(), Box<dyn std::error::Error>> {
        let mut file_system = super::VirtualFileSystem::new();
        let result = file_system.mount(Path::new("/"), Empty::new());
        assert_eq!(result, super::MountResult::Success);
        let result = file_system.mount(Path::new("/"), Empty::new());
        assert_eq!(result, super::MountResult::AlreadyMounted);
        Ok(())
    }

    #[test]
    #[traced_test]
    fn mount_with_overlapping_roots_works() -> Result<(), Box<dyn std::error::Error>> {
        let mut file_system = super::VirtualFileSystem::new();
        let result = file_system.mount(Path::new("/"), Empty::new());
        assert_eq!(result, super::MountResult::Success);
        let result = file_system.mount(Path::new("/test"), Empty::new());
        assert_eq!(result, super::MountResult::Success);
        Ok(())
    }

    #[test]
    #[traced_test]
    fn mount_with_different_paths_but_no_root_mount_works() -> Result<(), Box<dyn std::error::Error>> {
        let mut file_system = super::VirtualFileSystem::new();
        let result = file_system.mount(Path::new("/test"), Empty::new());
        assert_eq!(result, super::MountResult::Success);
        let result = file_system.mount(Path::new("/test2"), Empty::new());
        assert_eq!(result, super::MountResult::Success);
        Ok(())
    }

    #[test]
    #[traced_test]
    fn unmount_with_root_mounted_works() -> Result<(), Box<dyn std::error::Error>> {
        let mut file_system = super::VirtualFileSystem::new();
        let result = file_system.mount(Path::new("/"), Empty::new());
        assert_eq!(result, super::MountResult::Success);
        let result = file_system.unmount(Path::new("/"));
        assert_eq!(result, super::UnmountResult::Success);
        Ok(())
    }

    #[test]
    #[traced_test]
    fn unmount_with_nothing_mounted_fails() -> Result<(), Box<dyn std::error::Error>> {
        let mut file_system = super::VirtualFileSystem::new();
        let result = file_system.unmount(Path::new("/"));
        assert_eq!(result, super::UnmountResult::NotMounted);
        Ok(())
    }

    #[test]
    #[traced_test]
    fn unmount_with_something_mounted_at_different_path_fails() -> Result<(), Box<dyn std::error::Error>> {
        let mut file_system = super::VirtualFileSystem::new();
        let result = file_system.mount(Path::new("/test"), Empty::new());
        assert_eq!(result, super::MountResult::Success);
        let result = file_system.unmount(Path::new("/test2"));
        assert_eq!(result, super::UnmountResult::NotMounted);
        Ok(())
    }

    #[test]
    #[traced_test]
    fn unmount_with_something_mounted_on_non_root_will_fail_if_root_is_not_mounted() -> Result<(), Box<dyn std::error::Error>> {
        let mut file_system = super::VirtualFileSystem::new();
        let result = file_system.mount(Path::new("/test"), Empty::new());
        assert_eq!(result, super::MountResult::Success);
        let result = file_system.unmount(Path::new("/"));
        assert_eq!(result, super::UnmountResult::NotMounted);
        Ok(())
    }

    #[test]
    #[traced_test]
    fn resolve_with_nothing_mounted_yields_nothing_mounted_error() -> Result<(), Box<dyn std::error::Error>> {
        let file_system = super::VirtualFileSystem::new();
        let result = file_system.resolve(&Path::new("/"));
        assert!(result.is_err());
        assert!(result.is_err() && result.unwrap_err() == super::ResolveError::NothingMounted);
        Ok(())
    }

    #[test]
    #[traced_test]
    fn resolve_with_empty_mounted_yields_empty_mounted() -> Result<(), Box<dyn std::error::Error>> {
        let mut file_system = super::VirtualFileSystem::new();
        file_system.mount(Path::new("/"), Empty::new());
        let result = file_system.resolve(&Path::new("/"));
        assert!(result.is_ok());
        let (mount, path) = result.unwrap();
        assert_eq!(path, Path::new("/"));
        Ok(())
    }

    #[test]
    #[traced_test]
    fn resolve_with_empty_mounted_on_root_and_path_yields_empty_mounted_and_path() -> Result<(), Box<dyn std::error::Error>> {
        let mut file_system = super::VirtualFileSystem::new();
        file_system.mount(Path::new("/"), Empty::new());
        let result = file_system.resolve(&Path::new("/test"));
        assert!(result.is_ok());
        let (mount, path) = result.unwrap();
        assert_eq!(path, Path::new("/test"));
        Ok(())
    }

    #[test]
    #[traced_test]
    fn resolve_with_empty_mounted_on_path_yields_empty_mounted_and_subpath() -> Result<(), Box<dyn std::error::Error>> {
        let mut file_system = super::VirtualFileSystem::new();
        file_system.mount(Path::new("/mount/"), Empty::new());
        let result = file_system.resolve(Path::new("/mount/test/subtest"));
        assert!(result.is_ok());
        let (mount, path) = result.unwrap();
        assert_eq!(path, Path::new("/test/subtest"));
        Ok(())
    }

    #[test]
    #[traced_test]
    fn resolve_with_overlapping_mounts_yields_more_narrow_mount_with_correct_subpath() -> Result<(), Box<dyn std::error::Error>> {
        let mut file_system = super::VirtualFileSystem::new();

        let mut outer = Empty::new();
        assert_eq!(outer.set_property("test".into(), "outer".into()), Ok(()));
        file_system.mount(Path::new("/mount/"), outer);

        let mut inner = Empty::new();
        assert_eq!(inner.set_property("test".into(), "inner".into()), Ok(()));
        file_system.mount(Path::new("/mount/test/"), inner);

        let result = file_system.resolve(Path::new("/mount/outer"));
        assert!(result.is_ok());
        let (mount, path) = result.unwrap();
        assert_eq!(path, Path::new("/outer"));
        assert_eq!(mount.get_property("test".into()), Ok("outer".into()));

        let result = file_system.resolve(Path::new("/mount/test/inner"));
        assert!(result.is_ok());
        let (mount, path) = result.unwrap();
        assert_eq!(path, Path::new("/inner"));
        assert_eq!(mount.get_property("test".into()), Ok("inner".into()));
        Ok(())
    }

    #[test]
    #[traced_test]
    fn resolve_with_overlapping_mounts_yields_more_narrow_mount_with_correct_subpath_and_mount_order_does_not_matter() -> Result<(), Box<dyn std::error::Error>> {
        let mut file_system = super::VirtualFileSystem::new();

        let mut inner = Empty::new();
        assert_eq!(inner.set_property("test".into(), "inner".into()), Ok(()));
        file_system.mount(Path::new("/mount/test/"), inner);

        let mut outer = Empty::new();
        assert_eq!(outer.set_property("test".into(), "outer".into()), Ok(()));
        file_system.mount(Path::new("/mount/"), outer);

        let result = file_system.resolve(Path::new("/mount/outer"));
        assert!(result.is_ok());
        let (mount, path) = result.unwrap();
        assert_eq!(path, Path::new("/outer"));
        assert_eq!(mount.get_property("test".into()), Ok("outer".into()));

        let result = file_system.resolve(Path::new("/mount/test/inner"));
        assert!(result.is_ok());
        let (mount, path) = result.unwrap();
        assert_eq!(path, Path::new("/inner"));
        assert_eq!(mount.get_property("test".into()), Ok("inner".into()));
        Ok(())
    }

    #[test]
    #[traced_test]
    fn resolve_neither_forward_nor_backward_slash_matters() -> Result<(), Box<dyn std::error::Error>> {
        let mut file_system = super::VirtualFileSystem::new();

        let mut inner = Empty::new();
        assert_eq!(inner.set_property("test".into(), "inner".into()), Ok(()));
        file_system.mount(Path::new("/mount/test/"), inner);

        let mut outer = Empty::new();
        assert_eq!(outer.set_property("test".into(), "outer".into()), Ok(()));
        file_system.mount(Path::new("/mount/"), outer);

        let result = file_system.resolve(Path::new("/mount/outer"));
        assert!(result.is_ok());
        let (mount, path) = result.unwrap();
        assert_eq!(path, Path::new("/outer"));
        assert_eq!(mount.get_property("test".into()), Ok("outer".into()));

        let result = file_system.resolve(Path::new("/mount/test/inner"));
        assert!(result.is_ok());
        let (mount, path) = result.unwrap();
        assert_eq!(path, Path::new("/inner"));
        assert_eq!(mount.get_property("test".into()), Ok("inner".into()));

        let result = file_system.resolve(Path::new("\\mount\\outer"));
        assert!(result.is_ok());
        let (mount, path) = result.unwrap();
        assert_eq!(path, Path::new("/outer"));
        assert_eq!(mount.get_property("test".into()), Ok("outer".into()));

        let result = file_system.resolve(Path::new("\\mount\\test\\inner"));
        assert!(result.is_ok());
        let (mount, path) = result.unwrap();
        assert_eq!(path, Path::new("/inner"));
        assert_eq!(mount.get_property("test".into()), Ok("inner".into()));
        Ok(())
    }
}