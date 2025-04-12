unsafe extern "C" {
    pub fn debug(fmt: *const u8, ...);
    pub fn info(fmt: *const u8, ...);
    pub fn warn(fmt: *const u8, ...);
    pub fn error(fmt: *const u8, ...);
    pub fn fatal(fmt: *const u8, ...);
}

#[macro_export]
macro_rules! debug {
    ($fmt:expr) => {{ $crate::error::debug($fmt.as_ptr()); }};
    ($fmt:expr, $($arg:expr),*) => {{ $crate::error::debug($fmt.as_ptr(), ($($arg),*)); }};
}
#[macro_export]
macro_rules! info {
    ($fmt:expr) => {{ $crate::error::info($fmt.as_ptr()); }};
    ($fmt:expr, $($arg:expr),*) => {{ $crate::error::info($fmt.as_ptr(), ($($arg),*)); }};
}
#[macro_export]
macro_rules! warn {
    ($fmt:expr) => {{ $crate::error::warn($fmt.as_ptr()); }};
    ($fmt:expr, $($arg:expr),*) => {{ $crate::error::warn($fmt.as_ptr(), ($($arg),*)); }};
}
#[macro_export]
macro_rules! error {
    ($fmt:expr) => {{ $crate::error::error($fmt.as_ptr()); }};
    ($fmt:expr, $($arg:expr),*) => {{ $crate::error::error($fmt.as_ptr(), ($($arg),*)); }};
}
#[macro_export]
macro_rules! fatal {
    ($fmt:expr) => {{ $crate::error::fatal($fmt.as_ptr()); }};
    ($fmt:expr, $($arg:expr),*) => {{ $crate::error::fatal($fmt.as_ptr(), ($($arg),*)); }};
}
