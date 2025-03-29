const std = @import("std");

pub fn MyBufferStack(comptime T: type) type {
    return struct {
        buffer: []T,
        len: usize,

        const Self = @This();

        pub fn init(buffer: []T) Self {
            return Self{ .buffer = buffer, .len = 0 };
        }

        /// Panics if it goes out of bound
        pub fn push(self: *Self, val: T) void {
            self.buffer[self.len] = val;
            self.len += 1;
        }

        pub fn pop(self: *Self) ?T {
            if (self.len == 0) return null;
            self.len -= 1;
            return self.buffer[self.len];
        }

        pub fn slice(self: *Self) []T {
            return self.buffer[0..self.len];
        }
    };
}
