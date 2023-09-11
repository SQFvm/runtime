use std::cell::Ref;

#[derive(Debug, PartialEq, Eq, Clone, Copy)]
pub enum PathSeparator {
    ForwardSlash,
    BackwardSlash,
}

#[derive(Debug, PartialEq, Eq, Clone)]
pub struct Path {
    value: String,
}

#[derive(Debug, PartialEq, Eq, Clone, Copy)]
pub enum PathParentError {
    RootHasNoParent,
    NoSeparatorFound,
}

#[derive(Debug, PartialEq, Eq, Clone, Copy)]
pub enum PathNormalizationError {
    PathAboveRoot,
}

impl Into<String> for Path {
    fn into(self) -> String {
        self.value
    }
}

impl Into<String> for &Path {
    fn into(self) -> String {
        self.value.clone()
    }
}

impl From<String> for Path {
    fn from(value: String) -> Self {
        Path { value }
    }
}

impl From<&str> for Path {
    fn from(value: &str) -> Self {
        Path { value: value.to_string() }
    }
}

impl From<&String> for Path {
    fn from(value: &String) -> Self {
        Path { value: value.clone() }
    }
}

impl From<&Path> for Path {
    fn from(value: &Path) -> Self {
        Path { value: value.value.clone() }
    }
}


impl AsRef<str> for Path {
    fn as_ref(&self) -> &str {
        &self.value
    }
}

impl AsRef<Path> for Path {
    fn as_ref(&self) -> &Path {
        self
    }
}

impl AsRef<std::path::Path> for Path {
    fn as_ref(&self) -> &std::path::Path {
        self.value.as_ref()
    }
}

impl Path {
    /**
     * # Description
     * Create a new path from a string.
     *
     * # Arguments
     * - `value` The string to create the path from.
     */
    pub fn new(value: String) -> Path {
        Path { value }
    }

    /**
     * # Description
     * Creates a new empty path.
     */
    pub fn empty() -> Path {
        Path { value: String::new() }
    }

    /**
     * # Description
     * Convert the path to a string slice.
     */
    pub fn to_str(&self) -> &str {
        &self.value
    }

    /**
     * # Description
     * Convert the path to a string.
     */
    pub fn to_string(&self) -> String {
        self.value.clone()
    }

    /**
     * # Description
     * Get a reference to the underlying string.
     */
    pub fn ref_string(&self) -> &String {
        &self.value
    }

    /**
     * # Description
     * Iterate over the segments of the path.
     */
    pub fn iter_segments(&self) -> impl Iterator<Item=&str> {
        self.value.split(|c| c == '/' || c == '\\')
    }

    /**
     * # Description
     * Check if the path is the root.
     * This will return true if the path is "/" or "\" or "./" or ".\" or "X:/" or "X:\".
     */
    pub fn is_root(&self) -> bool {
        self.value == "/"
            || self.value == "\\"
            || self.value == "./"
            || self.value == ".\\"
            || (self.value.len() == 3 && self.value.ends_with(":/"))
            || (self.value.len() == 3 && self.value.ends_with(":\\"))
    }

    /**
     * # Description
     * Check if the path is absolute.
     * This will return true if the path starts with "/" or "\" or "X:/" or "X:\".
     */
    pub fn is_absolute(&self) -> bool {
        self.value.starts_with('/')
            || self.value.starts_with('\\')
            || (self.value.len() >= 3 && self.value[1..3].ends_with(":/"))
            || (self.value.len() >= 3 && self.value[1..3].ends_with(":\\"))
    }

    /**
     * # Description
     * Check if the path is relative.
     * This will return true if the path does not start with "/" or "\" or "X:/" or "X:\".
     */
    pub fn is_relative(&self) -> bool {
        !self.is_absolute()
    }

    /**
     * # Description
     * Check if the path is empty.
     * This will return true if the path is "" or ".".
     */
    pub fn is_empty(&self) -> bool {
        self.value.is_empty() || self.value == "."
    }

    /**
     * # Description
     * Get the parent of the path.
     * Note that double dots are not resolved.
     *
     * # Returns
     * A new path representing the parent of the path.
     */
    pub fn parent(&self) -> Result<Path, PathParentError> {
        if self.is_root() {
            return Err(PathParentError::RootHasNoParent);
        }
        let opt_last = self.value.rfind(|c| c == '/' || c == '\\');
        if let Some(last) = opt_last {
            Ok(Path::new(self.value[..last].to_string()))
        } else {
            Err(PathParentError::NoSeparatorFound)
        }
    }

    /**
     * # Description
     * Get the last segment of the path.
     * Note that double dots are not resolved.
     */
    pub fn last_segment(&self) -> &str {
        let opt_last = self.value.rfind(|c| c == '/' || c == '\\');
        if let Some(last) = opt_last {
            &self.value[last + 1..]
        } else {
            &self.value
        }
    }

    /**
     * # Description
     * Normalize the path by removing any redundant separators, changing all separators to the
     * one specified, and removing any trailing separators.
     * Any double dots are resolved by removing the previous segment or returning an error if
     * the path tries to go above the root.
     * If the path has a leading separator, it will be preserved.
     * If the path has a leading dot with a separator following it, it will be preserved.
     *
     * This function does not check if the path is valid.
     * It also will not make a path absolute if it is relative.
     * It neither resolves nor removes symlinks.
     *
     * # Arguments
     * - `separator` The separator to use.
     *
     * # Returns
     * A new path representing the normalized version of the path.
     */
    pub fn normalize(&self, separator: PathSeparator) -> Result<Path, PathNormalizationError> {
        let mut stack = vec![];
        let mut segments = self.iter_segments();
        if self.is_relative() && !(self.value.starts_with("./") || self.value.starts_with(".\\")) {
            stack.push(".")
        } else if self.is_absolute() {
            stack.push("")
        }
        let mut opt_next_segment = segments.next();
        while let Some(next_segment) = opt_next_segment {
            if next_segment.is_empty() || next_segment == "." {
                opt_next_segment = segments.next();
                continue;
            }
            if next_segment == ".." {
                if stack.is_empty() {
                    return Err(PathNormalizationError::PathAboveRoot);
                }
                stack.pop();
            } else {
                stack.push(next_segment);
            }
            opt_next_segment = segments.next();
        }

        match stack.len() {
            0 => Err(PathNormalizationError::PathAboveRoot),
            1 => Ok(Path::new(stack[0].to_string() + match separator {
                PathSeparator::ForwardSlash => "/",
                PathSeparator::BackwardSlash => "\\",
            })),
            _ => Ok(Path::new(stack.join(match separator {
                PathSeparator::ForwardSlash => "/",
                PathSeparator::BackwardSlash => "\\",
            })))
        }
    }

    /**
     * # Description
     * Check if the path starts with the specified path.
     *
     * # Arguments
     * - `other` The path to check if this path starts with.
     *
     * # Returns
     * Whether this path starts with the specified path.
     */
    pub fn starts_with(&self, other: &Path) -> bool {
        // Using segments
        let mut self_segments = self.iter_segments();
        let mut other_segments = other.iter_segments();
        let mut opt_self_segment = self_segments.next();
        let mut opt_other_segment = other_segments.next();
        while let Some(self_segment) = opt_self_segment {
            if let Some(other_segment) = opt_other_segment {
                if self_segment != other_segment {
                    return false;
                }
            } else {
                return true;
            }
            opt_self_segment = self_segments.next();
            opt_other_segment = other_segments.next();
        }
        opt_other_segment.is_none()
    }

    /**
     * # Description
     * Create a slice of this path from the specified start index to the specified end index.
     */
    pub fn slice(&self, separator: PathSeparator, start: Option<usize>, end: Option<usize>) -> Path {
        // Using segments
        let mut segments = self.iter_segments();
        let mut result = String::new();
        result.reserve(self.value.len());
        let mut opt_next_segment = segments.next();
        let mut index = 0;
        while let Some(next_segment) = opt_next_segment {
            if index >= end.unwrap_or(std::usize::MAX) {
                break;
            }
            if index >= start.unwrap_or(0) {
                result.push_str(next_segment);
            }
            opt_next_segment = segments.next();
            if opt_next_segment.is_some() {
                match separator {
                    PathSeparator::ForwardSlash => result.push('/'),
                    PathSeparator::BackwardSlash => result.push('\\'),
                }
            }
            index += 1;
        }
        Path::new(result)
    }
}


#[cfg(test)]
mod tests {
    use tracing_test::traced_test;
    use crate::path::{Path, PathNormalizationError, PathParentError};
    use crate::path::PathSeparator::{BackwardSlash, ForwardSlash};

    #[test]
    #[traced_test]
    fn test_iter_segments() -> Result<(), Box<dyn std::error::Error>> {
        let path = Path::new("a/b/c".to_string());
        let mut segments = path.iter_segments();
        assert_eq!(segments.next(), Some("a"));
        assert_eq!(segments.next(), Some("b"));
        assert_eq!(segments.next(), Some("c"));
        assert_eq!(segments.next(), None);
        Ok(())
    }

    #[test]
    #[traced_test]
    fn test_is_root() -> Result<(), Box<dyn std::error::Error>> {
        assert!(Path::new("/".to_string()).is_root());
        assert!(Path::new("\\".to_string()).is_root());
        assert!(Path::new("./".to_string()).is_root());
        assert!(Path::new(".\\".to_string()).is_root());
        assert!(Path::new("C:/".to_string()).is_root());
        assert!(Path::new("C:\\".to_string()).is_root());
        assert!(!Path::new("a".to_string()).is_root());
        assert!(!Path::new("a/b".to_string()).is_root());
        assert!(!Path::new("a/b/c".to_string()).is_root());
        Ok(())
    }

    #[test]
    #[traced_test]
    fn test_is_absolute() -> Result<(), Box<dyn std::error::Error>> {
        assert!(Path::new("/".to_string()).is_absolute());
        assert!(Path::new("\\".to_string()).is_absolute());
        assert!(Path::new("C:/".to_string()).is_absolute());
        assert!(Path::new("C:\\".to_string()).is_absolute());
        assert!(!Path::new("./".to_string()).is_absolute());
        assert!(!Path::new(".\\".to_string()).is_absolute());
        assert!(!Path::new("a".to_string()).is_absolute());
        assert!(!Path::new("a/b".to_string()).is_absolute());
        assert!(!Path::new("a/b/c".to_string()).is_absolute());
        assert!(Path::new("/a/b/c".to_string()).is_absolute());
        assert!(Path::new("\\a\\b\\c".to_string()).is_absolute());
        assert!(Path::new("C:/a/b/c".to_string()).is_absolute());
        assert!(Path::new("C:\\a\\b\\c".to_string()).is_absolute());
        Ok(())
    }

    #[test]
    #[traced_test]
    fn test_is_relative() -> Result<(), Box<dyn std::error::Error>> {
        assert!(!Path::new("/".to_string()).is_relative());
        assert!(!Path::new("\\\\".to_string()).is_relative());
        assert!(!Path::new("C:/".to_string()).is_relative());
        assert!(!Path::new("C:\\".to_string()).is_relative());
        assert!(Path::new("./".to_string()).is_relative());
        assert!(Path::new(".\\".to_string()).is_relative());
        assert!(Path::new("a".to_string()).is_relative());
        assert!(Path::new("a/b".to_string()).is_relative());
        assert!(Path::new("a/b/c".to_string()).is_relative());
        assert!(!Path::new("/a/b/c".to_string()).is_relative());
        assert!(!Path::new("\\\\a\\b\\c".to_string()).is_relative());
        assert!(!Path::new("C:/a/b/c".to_string()).is_relative());
        assert!(!Path::new("C:\\a\\b\\c".to_string()).is_relative());
        Ok(())
    }

    #[test]
    #[traced_test]
    fn test_is_empty() -> Result<(), Box<dyn std::error::Error>> {
        assert!(Path::new("".to_string()).is_empty());
        assert!(Path::new(".".to_string()).is_empty());
        assert!(!Path::new("..".to_string()).is_empty());
        assert!(!Path::new("/".to_string()).is_empty());
        assert!(!Path::new("\\\\".to_string()).is_empty());
        assert!(!Path::new("C:/".to_string()).is_empty());
        assert!(!Path::new("C:\\".to_string()).is_empty());
        assert!(!Path::new("./".to_string()).is_empty());
        assert!(!Path::new(".\\".to_string()).is_empty());
        assert!(!Path::new("a".to_string()).is_empty());
        assert!(!Path::new("a/b".to_string()).is_empty());
        assert!(!Path::new("a/b/c".to_string()).is_empty());
        Ok(())
    }

    #[test]
    #[traced_test]
    fn test_parent() -> Result<(), Box<dyn std::error::Error>> {
        assert_eq!(Path::new("a/b/c".to_string()).parent(), Ok(Path::new("a/b".to_string())));
        assert_eq!(Path::new("a/b/c/".to_string()).parent(), Ok(Path::new("a/b".to_string())));
        assert_eq!(Path::new("a/b/c/.".to_string()).parent(), Ok(Path::new("a/b/c".to_string())));
        assert_eq!(Path::new("a/b/c/./".to_string()).parent(), Ok(Path::new("a/b/c".to_string())));
        assert_eq!(Path::new("a/b/c/..".to_string()).parent(), Ok(Path::new("a/b/c".to_string())));
        assert_eq!(Path::new("a/b/c/../".to_string()).parent(), Ok(Path::new("a/b/c".to_string())));
        assert_eq!(Path::new("a/b/c/../..".to_string()).parent(), Ok(Path::new("a/b/c/..".to_string())));
        assert_eq!(Path::new("a/b/c/../../".to_string()).parent(), Ok(Path::new("a/b/c/..".to_string())));
        assert_eq!(Path::new("a/b/c/../../../".to_string()).parent(), Ok(Path::new("a/b/c/../..".to_string())));
        assert_eq!(Path::new("a/b/c/../../../../".to_string()).parent(), Ok(Path::new("a/b/c/../../..".to_string())));
        assert_eq!(Path::new("a".to_string()).parent(), Err(PathParentError::RootHasNoParent));
        assert_eq!(Path::new("a/".to_string()).parent(), Err(PathParentError::RootHasNoParent));
        assert_eq!(Path::new("a/.".to_string()).parent(), Ok(Path::new("a".to_string())));
        assert_eq!(Path::new("a/./".to_string()).parent(), Ok(Path::new("a".to_string())));
        assert_eq!(Path::new("a/..".to_string()).parent(), Ok(Path::new("a".to_string())));
        assert_eq!(Path::new("a/../".to_string()).parent(), Ok(Path::new("a".to_string())));
        assert_eq!(Path::new("a/../..".to_string()).parent(), Ok(Path::new("a/..".to_string())));
        assert_eq!(Path::new("a/../../".to_string()).parent(), Ok(Path::new("a/..".to_string())));
        assert_eq!(Path::new("a/../../../".to_string()).parent(), Ok(Path::new("a/../..".to_string())));
        assert_eq!(Path::new("a/../../../../".to_string()).parent(), Ok(Path::new("a/../../..".to_string())));
        assert_eq!(Path::new("/".to_string()).parent(), Err(PathParentError::RootHasNoParent));
        assert_eq!(Path::new("//".to_string()).parent(), Err(PathParentError::RootHasNoParent));
        assert_eq!(Path::new("C:/".to_string()).parent(), Err(PathParentError::RootHasNoParent));
        assert_eq!(Path::new("C:\\".to_string()).parent(), Err(PathParentError::RootHasNoParent));
        assert_eq!(Path::new("./".to_string()).parent(), Err(PathParentError::RootHasNoParent));
        assert_eq!(Path::new(".\\".to_string()).parent(), Err(PathParentError::RootHasNoParent));
        assert_eq!(Path::new(".".to_string()).parent(), Err(PathParentError::RootHasNoParent));
        assert_eq!(Path::new("..".to_string()).parent(), Err(PathParentError::RootHasNoParent));
        assert_eq!(Path::new("../".to_string()).parent(), Err(PathParentError::RootHasNoParent));
        assert_eq!(Path::new("../..".to_string()).parent(), Ok(Path::new("..".to_string())));
        assert_eq!(Path::new("../../".to_string()).parent(), Ok(Path::new("..".to_string())));
        assert_eq!(Path::new("../../../".to_string()).parent(), Ok(Path::new("../..".to_string())));
        assert_eq!(Path::new("../../../../".to_string()).parent(), Ok(Path::new("../../..".to_string())));
        Ok(())
    }

    #[test]
    #[traced_test]
    fn test_normalize() -> Result<(), Box<dyn std::error::Error>> {
        assert_eq!(Path::new("a/b/c".to_string()).normalize(ForwardSlash), Ok(Path::new("./a/b/c".to_string())));
        assert_eq!(Path::new("a\\b\\c".to_string()).normalize(ForwardSlash), Ok(Path::new("./a/b/c".to_string())));
        assert_eq!(Path::new("a/b/c".to_string()).normalize(BackwardSlash), Ok(Path::new(".\\a\\b\\c".to_string())));
        assert_eq!(Path::new("a\\b\\c".to_string()).normalize(BackwardSlash), Ok(Path::new(".\\a\\b\\c".to_string())));
        assert_eq!(Path::new("a/b/c/".to_string()).normalize(ForwardSlash), Ok(Path::new("./a/b/c".to_string())));
        assert_eq!(Path::new("a\\b\\c\\".to_string()).normalize(ForwardSlash), Ok(Path::new("./a/b/c".to_string())));
        assert_eq!(Path::new("a/b/c/".to_string()).normalize(BackwardSlash), Ok(Path::new(".\\a\\b\\c".to_string())));
        assert_eq!(Path::new("a\\b\\c\\".to_string()).normalize(BackwardSlash), Ok(Path::new(".\\a\\b\\c".to_string())));
        assert_eq!(Path::new("a/b/c/.".to_string()).normalize(ForwardSlash), Ok(Path::new("./a/b/c".to_string())));
        assert_eq!(Path::new("a\\b\\c\\.".to_string()).normalize(ForwardSlash), Ok(Path::new("./a/b/c".to_string())));
        assert_eq!(Path::new("a/b/c/.".to_string()).normalize(BackwardSlash), Ok(Path::new(".\\a\\b\\c".to_string())));
        assert_eq!(Path::new("a\\b\\c\\.".to_string()).normalize(BackwardSlash), Ok(Path::new(".\\a\\b\\c".to_string())));
        assert_eq!(Path::new("a/b/c/./".to_string()).normalize(ForwardSlash), Ok(Path::new("./a/b/c".to_string())));
        assert_eq!(Path::new("a\\b\\c\\.\\".to_string()).normalize(ForwardSlash), Ok(Path::new("./a/b/c".to_string())));
        assert_eq!(Path::new("a/b/c/./".to_string()).normalize(BackwardSlash), Ok(Path::new(".\\a\\b\\c".to_string())));
        assert_eq!(Path::new("a\\b\\c\\.\\".to_string()).normalize(BackwardSlash), Ok(Path::new(".\\a\\b\\c".to_string())));
        assert_eq!(Path::new("a/b/c/..".to_string()).normalize(ForwardSlash), Ok(Path::new("./a/b".to_string())));
        assert_eq!(Path::new("a\\b\\c\\..".to_string()).normalize(ForwardSlash), Ok(Path::new("./a/b".to_string())));
        assert_eq!(Path::new("a/b/c/..".to_string()).normalize(BackwardSlash), Ok(Path::new(".\\a\\b".to_string())));
        assert_eq!(Path::new("a\\b\\c\\..".to_string()).normalize(BackwardSlash), Ok(Path::new(".\\a\\b".to_string())));
        assert_eq!(Path::new("a/b/c/../".to_string()).normalize(ForwardSlash), Ok(Path::new("./a/b".to_string())));
        assert_eq!(Path::new("a\\b\\c\\..\\".to_string()).normalize(ForwardSlash), Ok(Path::new("./a/b".to_string())));
        assert_eq!(Path::new("a/b/c/../".to_string()).normalize(BackwardSlash), Ok(Path::new(".\\a\\b".to_string())));
        assert_eq!(Path::new("a\\b\\c\\..\\".to_string()).normalize(BackwardSlash), Ok(Path::new(".\\a\\b".to_string())));
        assert_eq!(Path::new("a/b/c/../..".to_string()).normalize(ForwardSlash), Ok(Path::new("./a".to_string())));
        assert_eq!(Path::new("a\\b\\c\\..\\..".to_string()).normalize(ForwardSlash), Ok(Path::new("./a".to_string())));
        assert_eq!(Path::new("a/b/c/../..".to_string()).normalize(BackwardSlash), Ok(Path::new(".\\a".to_string())));
        assert_eq!(Path::new("a\\b\\c\\..\\..".to_string()).normalize(BackwardSlash), Ok(Path::new(".\\a".to_string())));
        assert_eq!(Path::new("a/b/c/../../".to_string()).normalize(ForwardSlash), Ok(Path::new("./a".to_string())));
        assert_eq!(Path::new("a\\b\\c\\..\\..\\".to_string()).normalize(ForwardSlash), Ok(Path::new("./a".to_string())));
        assert_eq!(Path::new("a/b/c/../../".to_string()).normalize(BackwardSlash), Ok(Path::new(".\\a".to_string())));
        assert_eq!(Path::new("a\\b\\c\\..\\..\\".to_string()).normalize(BackwardSlash), Ok(Path::new(".\\a".to_string())));
        assert_eq!(Path::new("a/b/c/../../../".to_string()).normalize(ForwardSlash), Ok(Path::new("./".to_string())));
        assert_eq!(Path::new("a\\b\\c\\..\\..\\..\\".to_string()).normalize(ForwardSlash), Ok(Path::new("./".to_string())));
        assert_eq!(Path::new("a/b/c/../../../".to_string()).normalize(BackwardSlash), Ok(Path::new(".\\".to_string())));
        assert_eq!(Path::new("a\\b\\c\\..\\..\\..\\".to_string()).normalize(BackwardSlash), Ok(Path::new(".\\".to_string())));
        assert_eq!(Path::new("a/b/c/../../../../".to_string()).normalize(ForwardSlash), Err(PathNormalizationError::PathAboveRoot));
        assert_eq!(Path::new("/a/b/c".to_string()).normalize(ForwardSlash), Ok(Path::new("/a/b/c".to_string())));
        assert_eq!(Path::new("\\a\\b\\c".to_string()).normalize(ForwardSlash), Ok(Path::new("/a/b/c".to_string())));
        assert_eq!(Path::new("/a/b/c".to_string()).normalize(BackwardSlash), Ok(Path::new("\\a\\b\\c".to_string())));
        assert_eq!(Path::new("\\a\\b\\c".to_string()).normalize(BackwardSlash), Ok(Path::new("\\a\\b\\c".to_string())));
        assert_eq!(Path::new("/a/b/c/".to_string()).normalize(ForwardSlash), Ok(Path::new("/a/b/c".to_string())));
        assert_eq!(Path::new("\\a\\b\\c\\".to_string()).normalize(ForwardSlash), Ok(Path::new("/a/b/c".to_string())));
        assert_eq!(Path::new("/a/b/c/".to_string()).normalize(BackwardSlash), Ok(Path::new("\\a\\b\\c".to_string())));
        assert_eq!(Path::new("\\a\\b\\c\\".to_string()).normalize(BackwardSlash), Ok(Path::new("\\a\\b\\c".to_string())));
        assert_eq!(Path::new("/a/b/c/.".to_string()).normalize(ForwardSlash), Ok(Path::new("/a/b/c".to_string())));
        assert_eq!(Path::new("\\a\\b\\c\\.".to_string()).normalize(ForwardSlash), Ok(Path::new("/a/b/c".to_string())));
        assert_eq!(Path::new("/a/b/c/.".to_string()).normalize(BackwardSlash), Ok(Path::new("\\a\\b\\c".to_string())));
        assert_eq!(Path::new("\\a\\b\\c\\.".to_string()).normalize(BackwardSlash), Ok(Path::new("\\a\\b\\c".to_string())));
        assert_eq!(Path::new("/a/b/c/./".to_string()).normalize(ForwardSlash), Ok(Path::new("/a/b/c".to_string())));
        assert_eq!(Path::new("\\a\\b\\c\\.\\".to_string()).normalize(ForwardSlash), Ok(Path::new("/a/b/c".to_string())));
        assert_eq!(Path::new("/a/b/c/./".to_string()).normalize(BackwardSlash), Ok(Path::new("\\a\\b\\c".to_string())));
        assert_eq!(Path::new("\\a\\b\\c\\.\\".to_string()).normalize(BackwardSlash), Ok(Path::new("\\a\\b\\c".to_string())));
        assert_eq!(Path::new("/a/b/c/..".to_string()).normalize(ForwardSlash), Ok(Path::new("/a/b".to_string())));
        assert_eq!(Path::new("\\a\\b\\c\\..".to_string()).normalize(ForwardSlash), Ok(Path::new("/a/b".to_string())));
        assert_eq!(Path::new("/a/b/c/..".to_string()).normalize(BackwardSlash), Ok(Path::new("\\a\\b".to_string())));
        assert_eq!(Path::new("\\a\\b\\c\\..".to_string()).normalize(BackwardSlash), Ok(Path::new("\\a\\b".to_string())));
        assert_eq!(Path::new("/a/b/c/../".to_string()).normalize(ForwardSlash), Ok(Path::new("/a/b".to_string())));
        assert_eq!(Path::new("\\a\\b\\c\\..\\".to_string()).normalize(ForwardSlash), Ok(Path::new("/a/b".to_string())));
        assert_eq!(Path::new("/a/b/c/../".to_string()).normalize(BackwardSlash), Ok(Path::new("\\a\\b".to_string())));
        assert_eq!(Path::new("\\a\\b\\c\\..\\".to_string()).normalize(BackwardSlash), Ok(Path::new("\\a\\b".to_string())));
        assert_eq!(Path::new("/a/b/c/../..".to_string()).normalize(ForwardSlash), Ok(Path::new("/a".to_string())));
        assert_eq!(Path::new("\\a\\b\\c\\..\\..".to_string()).normalize(ForwardSlash), Ok(Path::new("/a".to_string())));
        assert_eq!(Path::new("/a/b/c/../..".to_string()).normalize(BackwardSlash), Ok(Path::new("\\a".to_string())));
        assert_eq!(Path::new("\\a\\b\\c\\..\\..".to_string()).normalize(BackwardSlash), Ok(Path::new("\\a".to_string())));
        assert_eq!(Path::new("/a/b/c/../../".to_string()).normalize(ForwardSlash), Ok(Path::new("/a".to_string())));
        assert_eq!(Path::new("\\a\\b\\c\\..\\..\\".to_string()).normalize(ForwardSlash), Ok(Path::new("/a".to_string())));
        assert_eq!(Path::new("/a/b/c/../../".to_string()).normalize(BackwardSlash), Ok(Path::new("\\a".to_string())));
        assert_eq!(Path::new("\\a\\b\\c\\..\\..\\".to_string()).normalize(BackwardSlash), Ok(Path::new("\\a".to_string())));
        assert_eq!(Path::new("/a/b/c/../../../".to_string()).normalize(ForwardSlash), Ok(Path::new("/".to_string())));
        assert_eq!(Path::new("\\a\\b\\c\\..\\..\\..\\".to_string()).normalize(ForwardSlash), Ok(Path::new("/".to_string())));
        assert_eq!(Path::new("/a/b/c/../../../".to_string()).normalize(BackwardSlash), Ok(Path::new("\\".to_string())));
        assert_eq!(Path::new("\\a\\b\\c\\..\\..\\..\\".to_string()).normalize(BackwardSlash), Ok(Path::new("\\".to_string())));
        assert_eq!(Path::new("/a/b/c/../../../../".to_string()).normalize(ForwardSlash), Err(PathNormalizationError::PathAboveRoot));

        assert_eq!(Path::new("C:/a/b/c".to_string()).normalize(BackwardSlash), Ok(Path::new("C:/a/b/c".to_string())));
        assert_eq!(Path::new("C:\\a\\b\\c".to_string()).normalize(BackwardSlash), Ok(Path::new("C:/a/b/c".to_string())));
        assert_eq!(Path::new("C:/a/b/c".to_string()).normalize(ForwardSlash), Ok(Path::new("C:\\a\\b\\c".to_string())));
        assert_eq!(Path::new("C:\\a\\b\\c".to_string()).normalize(ForwardSlash), Ok(Path::new("C:\\a\\b\\c".to_string())));
        assert_eq!(Path::new("C:/a/b/c/".to_string()).normalize(BackwardSlash), Ok(Path::new("C:/a/b/c".to_string())));
        assert_eq!(Path::new("C:\\a\\b\\c\\".to_string()).normalize(BackwardSlash), Ok(Path::new("C:/a/b/c".to_string())));
        assert_eq!(Path::new("C:/a/b/c/".to_string()).normalize(ForwardSlash), Ok(Path::new("C:\\a\\b\\c".to_string())));
        assert_eq!(Path::new("C:\\a\\b\\c\\".to_string()).normalize(ForwardSlash), Ok(Path::new("C:\\a\\b\\c".to_string())));
        assert_eq!(Path::new("C:/a/b/c/.".to_string()).normalize(BackwardSlash), Ok(Path::new("C:/a/b/c".to_string())));
        assert_eq!(Path::new("C:\\a\\b\\c\\.".to_string()).normalize(BackwardSlash), Ok(Path::new("C:/a/b/c".to_string())));
        assert_eq!(Path::new("C:/a/b/c/.".to_string()).normalize(ForwardSlash), Ok(Path::new("C:\\a\\b\\c".to_string())));
        assert_eq!(Path::new("C:\\a\\b\\c\\.".to_string()).normalize(ForwardSlash), Ok(Path::new("C:\\a\\b\\c".to_string())));
        assert_eq!(Path::new("C:/a/b/c/./".to_string()).normalize(BackwardSlash), Ok(Path::new("C:/a/b/c".to_string())));
        assert_eq!(Path::new("C:\\a\\b\\c\\.\\".to_string()).normalize(BackwardSlash), Ok(Path::new("C:/a/b/c".to_string())));
        assert_eq!(Path::new("C:/a/b/c/./".to_string()).normalize(ForwardSlash), Ok(Path::new("C:\\a\\b\\c".to_string())));
        assert_eq!(Path::new("C:\\a\\b\\c\\.\\".to_string()).normalize(ForwardSlash), Ok(Path::new("C:\\a\\b\\c".to_string())));
        assert_eq!(Path::new("C:/a/b/c/..".to_string()).normalize(BackwardSlash), Ok(Path::new("C:/a/b".to_string())));
        assert_eq!(Path::new("C:\\a\\b\\c\\..".to_string()).normalize(BackwardSlash), Ok(Path::new("C:/a/b".to_string())));
        assert_eq!(Path::new("C:/a/b/c/..".to_string()).normalize(ForwardSlash), Ok(Path::new("C:\\a\\b".to_string())));
        assert_eq!(Path::new("C:\\a\\b\\c\\..".to_string()).normalize(ForwardSlash), Ok(Path::new("C:\\a\\b".to_string())));
        assert_eq!(Path::new("C:/a/b/c/../".to_string()).normalize(BackwardSlash), Ok(Path::new("C:/a/b".to_string())));
        assert_eq!(Path::new("C:\\a\\b\\c\\..\\".to_string()).normalize(BackwardSlash), Ok(Path::new("C:/a/b".to_string())));
        assert_eq!(Path::new("C:/a/b/c/../".to_string()).normalize(ForwardSlash), Ok(Path::new("C:\\a\\b".to_string())));
        assert_eq!(Path::new("C:\\a\\b\\c\\..\\".to_string()).normalize(ForwardSlash), Ok(Path::new("C:\\a\\b".to_string())));
        assert_eq!(Path::new("C:/a/b/c/../..".to_string()).normalize(BackwardSlash), Ok(Path::new("C:/a".to_string())));
        assert_eq!(Path::new("C:\\a\\b\\c\\..\\..".to_string()).normalize(BackwardSlash), Ok(Path::new("C:/a".to_string())));
        Ok(())
    }

    #[test]
    #[traced_test]
    fn test_starts_with() -> Result<(), Box<dyn std::error::Error>> {
        assert!(Path::new("a/b/c".to_string()).starts_with(&Path::new("a/b/c".to_string())));
        assert!(Path::new("a/b/c".to_string()).starts_with(&Path::new("a/b".to_string())));
        assert!(Path::new("a/b/c".to_string()).starts_with(&Path::new("a".to_string())));

        assert!(!Path::new("a/b/c".to_string()).starts_with(&Path::new("a/b/c".to_string())));
        assert!(!Path::new("a/b".to_string()).starts_with(&Path::new("a/b/c".to_string())));
        assert!(!Path::new("a".to_string()).starts_with(&Path::new("a/b/c".to_string())));
        Ok(())
    }

    #[test]
    #[traced_test]
    fn test_slice() -> Result<(), Box<dyn std::error::Error>> {
        assert_eq!(Path::new("a/b/c".to_string()).slice(ForwardSlash, None, None), Path::new("a/b/c".to_string()));
        assert_eq!(Path::new("a/b/c".to_string()).slice(ForwardSlash, Some(0), None), Path::new("a/b/c".to_string()));
        assert_eq!(Path::new("a/b/c".to_string()).slice(ForwardSlash, Some(0), Some(3)), Path::new("a/b/c".to_string()));
        assert_eq!(Path::new("a/b/c".to_string()).slice(ForwardSlash, Some(0), Some(2)), Path::new("a/b".to_string()));
        assert_eq!(Path::new("a/b/c".to_string()).slice(ForwardSlash, Some(0), Some(1)), Path::new("a".to_string()));
        assert_eq!(Path::new("a/b/c".to_string()).slice(ForwardSlash, Some(1), None), Path::new("b/c".to_string()));
        assert_eq!(Path::new("a/b/c".to_string()).slice(ForwardSlash, Some(1), Some(2)), Path::new("b".to_string()));
        assert_eq!(Path::new("a/b/c".to_string()).slice(ForwardSlash, Some(2), None), Path::new("c".to_string()));
        assert_eq!(Path::new("a/b/c".to_string()).slice(ForwardSlash, Some(2), Some(3)), Path::new("c".to_string()));
        assert_eq!(Path::new("a/b/c".to_string()).slice(ForwardSlash, Some(3), None), Path::new("".to_string()));
        assert_eq!(Path::new("a/b/c".to_string()).slice(ForwardSlash, Some(3), Some(3)), Path::new("".to_string()));
        assert_eq!(Path::new("a/b/c".to_string()).slice(ForwardSlash, Some(4), None), Path::new("".to_string()));
        assert_eq!(Path::new("a/b/c".to_string()).slice(ForwardSlash, Some(4), Some(3)), Path::new("".to_string()));
        assert_eq!(Path::new("a/b/c".to_string()).slice(ForwardSlash, Some(4), Some(4)), Path::new("".to_string()));

        assert_eq!(Path::new("a/b/c".to_string()).slice(BackwardSlash, None, None), Path::new("a\\b\\c".to_string()));
        assert_eq!(Path::new("a/b/c".to_string()).slice(BackwardSlash, Some(0), None), Path::new("a\\b\\c".to_string()));
        assert_eq!(Path::new("a/b/c".to_string()).slice(BackwardSlash, Some(0), Some(3)), Path::new("a\\b\\c".to_string()));
        assert_eq!(Path::new("a/b/c".to_string()).slice(BackwardSlash, Some(0), Some(2)), Path::new("a\\b".to_string()));
        assert_eq!(Path::new("a/b/c".to_string()).slice(BackwardSlash, Some(0), Some(1)), Path::new("a".to_string()));
        assert_eq!(Path::new("a/b/c".to_string()).slice(BackwardSlash, Some(1), None), Path::new("b\\c".to_string()));
        assert_eq!(Path::new("a/b/c".to_string()).slice(BackwardSlash, Some(1), Some(2)), Path::new("b".to_string()));
        assert_eq!(Path::new("a/b/c".to_string()).slice(BackwardSlash, Some(2), None), Path::new("c".to_string()));
        assert_eq!(Path::new("a/b/c".to_string()).slice(BackwardSlash, Some(2), Some(3)), Path::new("c".to_string()));
        assert_eq!(Path::new("a/b/c".to_string()).slice(BackwardSlash, Some(3), None), Path::new("".to_string()));
        assert_eq!(Path::new("a/b/c".to_string()).slice(BackwardSlash, Some(3), Some(3)), Path::new("".to_string()));
        assert_eq!(Path::new("a/b/c".to_string()).slice(BackwardSlash, Some(4), None), Path::new("".to_string()));
        assert_eq!(Path::new("a/b/c".to_string()).slice(BackwardSlash, Some(4), Some(3)), Path::new("".to_string()));
        assert_eq!(Path::new("a/b/c".to_string()).slice(BackwardSlash, Some(4), Some(4)), Path::new("".to_string()));

        Ok(())
    }
}