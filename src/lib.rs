#![cfg_attr(not(test), no_std)]

#[cfg(not(test))]
#[panic_handler]
fn panic(_info: &core::panic::PanicInfo) -> ! {
    loop {}
}

// contains all publicly facing functions

mod error;

pub extern "C" fn test() {
    unsafe {
        debug!("%s", "hi");
        info!("%s", "hi");
        warn!("%s", "hi");
        error!("%s", "hi");
        fatal!("%s", "hi");
    }
}
