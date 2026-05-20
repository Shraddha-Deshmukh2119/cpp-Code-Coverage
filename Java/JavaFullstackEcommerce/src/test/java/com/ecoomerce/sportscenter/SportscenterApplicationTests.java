package com.ecoomerce.sportscenter;

import com.ecoomerce.sportscenter.repository.BasketRepository;
import org.junit.jupiter.api.Test;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.boot.test.mock.mockito.MockBean;
import org.springframework.test.context.ActiveProfiles;

@SpringBootTest
@ActiveProfiles("test")
class SportscenterApplicationTests {

	@MockBean
	private BasketRepository basketRepository;

	@Test
	void contextLoads() {
	}

}
