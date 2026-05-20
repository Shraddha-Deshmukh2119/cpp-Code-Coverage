package com.ecoomerce.sportscenter.config;

import org.junit.jupiter.api.Test;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.security.core.userdetails.UserDetailsService;
import org.springframework.security.crypto.password.PasswordEncoder;

import static org.assertj.core.api.Assertions.assertThat;

class MyConfigTest {

    private final MyConfig myConfig = new MyConfig();

    @Test
    void userDetailsService_loadsInMemoryUser() {
        UserDetailsService userDetailsService = myConfig.userDetailsService();
        UserDetails user = userDetailsService.loadUserByUsername("rahul");

        assertThat(user.getUsername()).isEqualTo("rahul");
        assertThat(user.getAuthorities()).isNotEmpty();
    }

    @Test
    void passwordEncoder_matchesEncodedPassword() {
        PasswordEncoder encoder = myConfig.passwordEncoder();
        assertThat(encoder.matches("Password", encoder.encode("Password"))).isTrue();
    }
}
