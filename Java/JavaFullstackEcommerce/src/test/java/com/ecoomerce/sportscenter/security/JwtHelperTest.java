package com.ecoomerce.sportscenter.security;

import org.junit.jupiter.api.Test;
import org.springframework.security.core.userdetails.User;
import org.springframework.security.core.userdetails.UserDetails;

import java.util.Date;

import static org.assertj.core.api.Assertions.assertThat;

class JwtHelperTest {

    private final JwtHelper jwtHelper = new JwtHelper();

    @Test
    void generateToken_andValidate_succeeds() {
        UserDetails userDetails = User.withUsername("rahul")
                .password("pass")
                .roles("ADMIN")
                .build();

        String token = jwtHelper.generateToken(userDetails);

        assertThat(token).isNotBlank();
        assertThat(jwtHelper.getUserNameFromToken(token)).isEqualTo("rahul");
        assertThat(jwtHelper.validateToken(token, userDetails)).isTrue();
    }

    @Test
    void getExpirationDateFromToken_returnsFutureDate() {
        UserDetails userDetails = User.withUsername("rahul")
                .password("pass")
                .roles("ADMIN")
                .build();

        String token = jwtHelper.generateToken(userDetails);
        Date expiration = jwtHelper.getExpirationDateFromToken(token);

        assertThat(expiration).isAfter(new Date());
    }

    @Test
    void validateToken_failsForWrongUser() {
        UserDetails userDetails = User.withUsername("rahul")
                .password("pass")
                .roles("ADMIN")
                .build();
        UserDetails otherUser = User.withUsername("other")
                .password("pass")
                .roles("ADMIN")
                .build();

        String token = jwtHelper.generateToken(userDetails);

        assertThat(jwtHelper.validateToken(token, otherUser)).isFalse();
    }
}
