pipeline {

    agent any

    tools {
        jdk 'JDK25'
        maven 'maven'
    }

    environment {

        JAVA_PROJECT = "Java/JavaFullstackEcommerce"

        CPP_PROJECT = "Cpp"

        PYTHON_PARSER = "parser/parser.py"

        SONAR_TOKEN = credentials('sonar-token')
    }

    stages {

        // =========================
        // CLEAN
        // =========================
        stage('Clean Workspace') {

            steps {
                cleanWs()
            }
        }

        // =========================
        // CLONE
        // =========================
        stage('Clone Repository') {

            steps {
                git branch: 'main',
                url: 'https://github.com/Prathamesh-Jawahire/java.git'
            }
        }

        // =========================
        // JAVA BUILD
        // =========================
        stage('Java Build & Coverage') {

            steps {

                dir("${JAVA_PROJECT}") {

                    bat """
                    mvn clean verify
                    """
                }
            }
        }

        // =========================
        // JAVA SONAR
        // =========================
        stage('Java SonarCloud Analysis') {

            steps {

                dir("${JAVA_PROJECT}") {

                    withSonarQubeEnv('SonarCloud') {

                        bat """
                        mvn sonar:sonar ^
                        -Dsonar.projectKey=bmc-java ^
                        -Dsonar.organization=prathamesh-jawahire ^
                        -Dsonar.host.url=https://sonarcloud.io ^
                        -Dsonar.token=%SONAR_TOKEN% ^
                        -Dsonar.coverage.jacoco.xmlReportPaths=target/site/jacoco/jacoco.xml
                        """
                    }
                }
            }
        }

        // =========================
        // CPP BUILD
        // =========================
        stage('CPP Build & Coverage') {

            steps {

                dir("${CPP_PROJECT}") {

                    bat """
                    if exist build rmdir /s /q build

                    mkdir build

                    cd build

                    cmake ..

                    cmake --build .

                    ctest --output-on-failure

                    gcovr -x -o coverage.xml

                    gcovr --json -o coverage.json
                    """
                }
            }
        }

        // =========================
        // PARSER
        // =========================
        stage('Generate Unified JSON') {

            steps {

                bat """
                python ${PYTHON_PARSER} ^
                --jacoco_xml Java/JavaFullstackEcommerce/target/site/jacoco/jacoco.xml ^
                --surefire_dir Java/JavaFullstackEcommerce/target/surefire-reports ^
                --gcovr_xml Cpp/build/coverage.xml ^
                --gcovr_json Cpp/build/coverage.json ^
                --output unified_report.json
                """
            }
        }

        // =========================
        // ARCHIVE
        // =========================
        stage('Archive Reports') {

            steps {

                archiveArtifacts artifacts: '''
                unified_report.json,
                Java/JavaFullstackEcommerce/target/site/jacoco/**,
                Cpp/build/coverage.xml,
                Cpp/build/coverage.json
                ''',
                fingerprint: true
            }
        }
    }

    post {

        always {

            echo "Pipeline Finished."
        }

        success {

            echo "Build Success."
        }

        failure {

            echo "Build Failed."
        }
    }
}